/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 21:15:07
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 21:52:27
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/src/server/chatservice.cpp
 * @Descripttion: 
 */

#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <vector>
using namespace std;
using namespace muduo;

// 获取单例对象的接口函数
ChatService *ChatService::instance() {
    static ChatService service;
    return &service;
}

// 注册消息以及对应的Handler回调操作  绑定和回调，这是oop面向对象很重要的环节
ChatService::ChatService() {
    
    // 将消息id和对应的事件处理器绑定起来，不绑定的话消息派发不出去，没有绑定事件处理器
    // 用户基本业务管理相关事件处理回调注册
    _msgHandlerMap.insert({LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3)});
    _msgHandlerMap.insert({LOGINOUT_MSG, std::bind(&ChatService::loginout, this, _1, _2, _3)});
    _msgHandlerMap.insert({REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3)});
    _msgHandlerMap.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3)});

    // 群组业务管理相关事件处理回调注册
    _msgHandlerMap.insert({CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3)});
    _msgHandlerMap.insert({GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3)});

    // 连接redis服务器
    
}

// 获取消息对应的处理器
MsgHandler ChatService::getHandler(int msgid) {
    
    // 记录错误日志，msgid没有对应的事件处理回调
    auto it = _msgHandlerMap.find(msgid);
    if (it == _msgHandlerMap.end()) {
        // 返回一个默认的处理器，空操作    lambda表达式，[=]就是捕获（拷贝）所使用的来自所在函数的实体的值，包括msgid在内的函数所有变量
        return [=](const TcpConnectionPtr &conn, json &js, Timestamp) {
            LOG_ERROR << "msgid:" << msgid << " can not find handler!";
        };
    } else {
        return _msgHandlerMap[msgid];
    }
}

// 处理登录业务
void ChatService::login(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    LOG_INFO << "do login service!!!";

    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = _userModel.query(id);
    if (user.getId() == id && user.getPwd() == pwd) {
        if (user.getState() == "online") {
            // 该用户已经登录，不允许重复登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "this account is using, input another!";
            conn->send(response.dump());
            
        } else {
            // 有的用户上线，有的用户下线，就是在多线程访问，就要考虑线程安全
            // 登录成功，记录用户连接信息
            {
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.insert({id, conn});
            }
            // 这个大括号是一个作用域，进入大括号加锁，出大括号解锁，insert()就是临界区间
            // 锁的力度一定要小，不要扩大，不要加太多锁，不然多线程就变成单线程，变成串行，就没有体现出来并发编程的优势了
            
            // 数据库的并发操作是由mysql保证的，这个我不用管
            // 登录成功，更新用户状态信息 state offline=>online
            user.setState("online"); // 要集群，看用户是否在线，所以需要是否在线这个状态
            _userModel.updateState(user);

            // 登录成功，记录用户连接信息
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();
            
            // 查询该用户是否有离线消息
            vector<string> vec = _offlineMsgModel.query(id);
            if (!vec.empty()) {
                response["offlinemsg"] = vec;
                // 读取该用户的离线消息后，把该用户的所有离线消息删除掉
                _offlineMsgModel.remove(id);
            }

            // 查询该用户的好友信息并返回
            vector<User> userVec = _friendModel.query(id);
            if (!userVec.empty()) {
                vector<string> vec2;
                for (User &user : userVec) {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            conn->send(response.dump());
        }     
    } else {
        // 该用户不存在，用户存在但是密码错误，登录失败
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "id or password is invalid!";
        conn->send(response.dump());
    }
}

// 处理注册业务 name password
void ChatService::reg(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    LOG_INFO << "do reg service!!!";
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _userModel.insert(user);
    if (state) {
        // 注册成功
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    } else {
        // 注册失败
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

// 一对一聊天业务
void ChatService::oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int toid = js["toid"].get<int>();

    // 之后集群的时候这里的业务代码还要再修改一下，因为用户可能注册在不同的主机上
    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if (it != _userConnMap.end()) {
            // toid在线，转发消息   服务器主动推送消息给toid用户
            it->second->send(js.dump());
            return;
        }
    }

    // // 查询toid是否在线 
    // User user = _userModel.query(toid);
    // if (user.getState() == "online")
    // {
    //     _redis.publish(toid, js.dump());
    //     return;
    // }

    // toid不在线，存储离线消息
    _offlineMsgModel.insert(toid, js.dump());
}

// 添加好友业务
void ChatService::addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time) {
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    // 存储好友信息
    _friendModel.insert(userid, friendid);
}

// 创建群组业务
void ChatService::createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {

}

// 加入群组业务
void ChatService::addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time) {

}

// 群组聊天业务
void ChatService::groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time) {

}

// 处理注销业务
void ChatService::loginout(const TcpConnectionPtr &conn, json &js, Timestamp time) {

}

// 处理客户端异常退出
// TcpConnectionPtrs是一个只能指针
void ChatService::clientCloseException(const TcpConnectionPtr &conn) {
    User user;
    {
        lock_guard<mutex> lock(_connMutex); // 操作map，要注意线程安全
        for (auto it = _userConnMap.begin(); it != _userConnMap.end(); ++it) {
            if (it->second == conn) {
                // 从map表删除用户的链接信息
                user.setId(it->first);
                _userConnMap.erase(it);
                break;
            }
        }
    }

    // 用户注销，相当于就是下线，在redis中取消订阅通道
    // _redis.unsubscribe(user.getId()); 

    // 更新用户的状态信息
    if (user.getId() != -1) {
        user.setState("offline");
        _userModel.updateState(user);
    }
}

// 服务器异常，业务重置方法
void ChatService::reset() {
    // 把online状态的用户，设置成offline
    _userModel.resetState();
}
