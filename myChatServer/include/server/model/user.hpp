/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-11 13:37:51
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-13 10:38:24
 * @FilePath: /Linux_nc/ChatServer/myChatServer/include/server/user.hpp
 * @Descripttion: 
 */
#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// User表的ORM类  映射关系类，业务上操作user对象，不会影响mysql
class User
{
public:
    User(int id = -1, string name = "", string pwd = "", string state = "offline")
    {
        this->id = id;
        this->name = name;
        this->password = pwd;
        this->state = state;
    }

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPwd(string pwd) { this->password = pwd; }
    void setState(string state) { this->state = state; }

    int getId() { return this->id; }
    string getName() { return this->name; }
    string getPwd() { return this->password; }
    string getState() { return this->state; }

protected:
    int id;
    string name;
    string password;
    string state;
};

#endif