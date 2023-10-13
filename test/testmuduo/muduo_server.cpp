/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-09 20:03:40
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-09 22:04:34
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/testmuduo/muduo_server.cpp
 * @Descripttion: 
 */

// muduo网络库的编程很容易，要实现基于muduo网络库的服务器和客户端程序，
// 只需要简单的组合TcpServer和TcpClient就可以

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*服务器类，基于muduo库开发
 */

/* 基于muduo网络库开发服务器程序
1. 组合TcpServer对象
2. 创建EventLoop事件循环对象的指针
3. 明确TcpServer构造函数需要什么参数，输出chatServer的构造函数
4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量，muduo库会自己分配I/0线程和worker线程 
*/
class ChatServer
{
public:
    // 初始化TcpServer
    ChatServer(muduo::net::EventLoop *loop,                 // 事件循环
               const muduo::net::InetAddress &listenAddr)   // IP + Port
        : _server(loop, listenAddr, "ChatServer")           // ChatServer是服务器的名字
    {
        // 通过绑定器设置回调函数
        // 调用了这个回调函数，说明这个事件发生了，我不用关注事件是什么时候发生的，我只用关注事件发生之后要做的操作
        // 给服务器注册用户连接的创建和断开回调
        _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));
        // 给服务器注册用户读写事件回调
        _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));
        // 设置EventLoop的线程个数，设置服务器端的线程数量
        _server.setThreadNum(10);
    }
    
    // 启动ChatServer服务 
    void start() {
        _server.start();
    }

private:
    // TcpServer绑定的回调函数，当有新连接或连接中断时调用，专门处理用户的连接创建和断开
    // 在成员函数中，除了显式声明的参数之外，还有一个隐式的 this 指针，用于指向当前对象的实例，因此bind要加一个this
    void onConnection(const muduo::net::TcpConnectionPtr &conn) {
        if (conn->connected()) {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:online " << endl;
        } else {
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:offline " << endl;
            conn->shutdown(); // close(fd)
        }  
    }
    // TcpServer绑定的回调函数，当有新数据时调用，处理用户读写事件
    void onMessage(const muduo::net::TcpConnectionPtr &conn,    // 连接
                   muduo::net::Buffer *buffer,                     // 缓冲区
                   muduo::Timestamp time) {                      // 接收到数据的时间信息
        string buf = buffer->retrieveAllAsString();
        cout << "recv data:" << buf << "time:" << time.toString() << endl;
        conn->send(buf);
    }

private:
    muduo::net::TcpServer _server;
};

int main() {
    EventLoop loop; // epoll
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr);

    server.start();  // listenfd epoll_ctl=>epoll
    loop.loop(); // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等

    return 0;
}