/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 17:05:56
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-24 17:03:17
 * @FilePath: /Linux_nc/ChatServer/myChatServer/src/server/main.cpp
 * @Descripttion: 
 */

#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

// 处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int) {
    ChatService::instance()->reset();
    exit(0);
}

int main (int argc, char *argv[]) {

    if (argc < 3) {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000  or  ./ChatServer 127.0.0.1 6002" << endl;
        exit(-1);
    }

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT, resetHandler); // resetHandler是重置的方法

    EventLoop loop;
    // InetAddress addr("127.0.0.1", 6000);
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "ChatServer");

    server.start(); // listenfd epoll_ctl=>epoll
    loop.loop();    // epoll_wait以阻塞方式等待新用户连接，已连接用户的读写事件等
                    // loop.loop()之后，等待客户端的连接

    return 0;
}