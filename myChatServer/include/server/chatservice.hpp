/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 17:21:30
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-25 09:05:00
 * @FilePath: /Linux_nc/ChatServer/myChatServer/include/server/chatservice.hpp
 * @Descripttion: 
 */

// 服务 业务
// 采用单例模式

#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "redis.hpp"
#include "groupmodel_z.hpp"
#include "friendmodel.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "json.hpp"
using json = nlohmann::json;

// 事件处理器
// 表示处理消息的事件回调方法类型   MsgHandler相当于是一个数据类型    
using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

// 业务模块代码
// 聊天服务器业务类
class ChatService {
    
public:
    // 获取单例对象的接口函数
    static ChatService *instance();                             // 非回调函数
    // 处理登录业务
    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 添加好友业务
    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理注销业务
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr &conn);    // 非回调函数
    // 服务器异常，业务重置方法
    void reset();                                               // 非回调函数
    // 获取消息对应的处理器
    MsgHandler getHandler(int msgid);                           // 非回调函数
    
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);

private:
    ChatService();

    // 存储消息id和其对应的业务事件处理方法
    // 消息处理器的表，在里面写消息id和对应的处理操作
    unordered_map<int, MsgHandler> _msgHandlerMap;
    // 存储在线用户的通信连接
    unordered_map<int, TcpConnectionPtr> _userConnMap;
    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel; // 数据库相关的操作都封装在userModel里
    OfflineMsgModel _offlineMsgModel; // 此对象对离线消息进行增删查操作
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;
};

#endif