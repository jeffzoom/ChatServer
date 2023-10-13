/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 17:05:56
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-13 10:37:12
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

int main () {

    signal(SIGINT, resetHandler); // resetHandler是重置的方法

    EventLoop loop;
    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}