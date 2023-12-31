/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 15:44:51
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-24 20:52:08
 * @FilePath: /Linux_nc/ChatServer/myChatServer/src/server/chatserver.cpp
 * @Descripttion: 
 */
#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json; // 起一个更短的名称

// 网络模块代码
// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    // 注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量 一个主线程，I/O线程，main reactor，用于监听新用户的连接，三个工作线程，处理已连接用户的读写事件
    // 线程数量和cup核数对等，可以尽量做到高并发，I/O复用的好处：一个线程可以监听多个套接字，尤其是对于连接量大活跃量少的场景（一般都这样），epoll有非常大的优势
    // 另外，耗时的I/O操作，如传送文件，音视频，要单独开辟工作线程来处理；如果还是在epoll工作线程上处理耗时的IO操作的话，那当前这个epoll可能就无法及时的处理其他依然注册在这个epoll上的这个fd，就是socket的读写事件了
    _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start() {
    _server.start();
}

// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn) {
    // 客户端断开链接
    cout << " onConnection_zsq " << endl;
    if (!conn->connected()) {
        ChatService::instance()->clientCloseException(conn); // 可能客户端异常断开，要处理异常
        conn->shutdown();
    }
}

// 这个函数本身就会被多个线程调用，不同的用户在不同的工作线程中响应
// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time) 
{
    string buf = buffer->retrieveAllAsString();

    // 测试，添加json打印代码
    cout << buf << " onMessage_zsq " << endl;

    // 数据的反序列化 数据解码
    json js = json::parse(buf);
    // 达到的目的：完全解耦网络模块的代码和业务模块的代码   妙啊，现在只用考虑业务代码了，而且代码要分块测试，写完网络模块的代码就要测试一下
    // 通过js["msgid"] 获取=》业务handler=》conn  js  time          MsgHandler包含这三个参数
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    // 回调消息绑定好的事件处理器，来执行相应的业务处理
    msgHandler(conn, js, time);
}
