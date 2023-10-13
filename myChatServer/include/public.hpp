/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-10 17:50:43
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-13 14:52:11
 * @FilePath: /Linux_nc/ChatServer/myChatServer/include/public.hpp
 * @Descripttion: 
 */
#ifndef PUBLIC_H
#define PUBLIC_H

// 业务模块代码
/*
server和client的公共文件
*/
enum EnMsgType {
    
    LOGIN_MSG = 1,      // 登录消息 void login(const TcpConnectionPtr &conn, json &js, Timestamp time);
    LOGIN_MSG_ACK,      // 登录响应消息，响应消息，就是一般的后面再加一个ACK
    LOGINOUT_MSG,       // 注销消息
    REG_MSG,            // 注册消息 这个代表第4个 enum里面指定LOGIN_MSG是1，后面的就会依次排列  void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);
    REG_MSG_ACK,        // 注册响应消息
    ONE_CHAT_MSG,       // 聊天消息，一对一聊天
    ADD_FRIEND_MSG,     // 添加好友消息

    CREATE_GROUP_MSG,   // 创建群组
    ADD_GROUP_MSG,      // 加入群组
    GROUP_CHAT_MSG,     // 群聊天
};

#endif