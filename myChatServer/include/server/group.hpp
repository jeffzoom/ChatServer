/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 22:48:22
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 23:03:16
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/group.hpp
 * @Descripttion: 
 */
#ifndef GROUP_H
#define GROUP_H

#include "groupuser.hpp"
#include <string>
#include <vector>
using namespace std;

// User表的ORM类
class Group
{
public:
    Group(int id = -1, string name = "", string desc = "")
    {
        this->id = id;
        this->name = name;
        this->desc = desc;
    }

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setDesc(string desc) { this->desc = desc; }

    int getId() { return this->id; }
    string getName() { return this->name; }
    string getDesc() { return this->desc; }
    vector<GroupUser> &getUsers() { return this->users; }

private:
    int id;         // 组的id
    string name;    // 组的name
    string desc;    // 组的desc
    vector<GroupUser> users; // 需要知道一个组里有多少成员
};

#endif