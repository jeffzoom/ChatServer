/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 22:48:38
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 23:14:40
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/groupmodel.hpp
 * @Descripttion: 
 */
#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include <string>
#include <vector>
using namespace std;

// 维护群组信息的操作接口方法
class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group &group);
    // 加入群组
    void addGroup(int userid, int groupid, string role);
    // 查询用户所在群组信息
    vector<Group> queryGroups(int userid);
    // 根据指定的groupid查询群组用户id列表，除userid自己，主要用户群聊业务给群组其它成员群发消息
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif