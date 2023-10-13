/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 15:54:01
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-13 10:38:32
 * @FilePath: /Linux_nc/ChatServer/myChatServer/include/server/offlinemessagemodel.hpp
 * @Descripttion: 
 */
#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H

#include <string>
#include <vector>
using namespace std;

// 提供离线消息表的操作接口方法
class OfflineMsgModel
{
public:
    // 存储用户的离线消息
    void insert(int userid, string msg);

    // 删除用户的离线消息
    void remove(int userid);

    // 查询用户的离线消息
    vector<string> query(int userid);
};

#endif