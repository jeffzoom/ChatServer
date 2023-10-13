/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 22:48:38
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-13 10:17:27
 * @FilePath: /Linux_nc/ChatServer/myChatServer/include/server/groupmodel.hpp
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
    // 查询用户所在群组信息 给某一个成功登录的用户查看当前用户号，昵称，在线状态，群管理还是群员
    vector<Group> queryGroups(int userid);
    // 根据指定的groupid查询群组用户id列表，除userid自己，主要用户群聊业务给群组其它成员群发消息
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif