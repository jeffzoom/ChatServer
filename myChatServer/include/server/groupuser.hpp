/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 22:48:57
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 23:10:25
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/groupuser.hpp
 * @Descripttion: 
 */
#ifndef GROUPUSER_H
#define GROUPUSER_H

#include "user.hpp"

// 群组用户，多了一个role角色信息，从User类直接继承，复用User的其它信息
class GroupUser : public User {

public:
    void setRole(string role) { this->role = role; }
    string getRole() { return this->role; }

private:
    string role; // 在群组中，这个用户的角色信息
};

#endif