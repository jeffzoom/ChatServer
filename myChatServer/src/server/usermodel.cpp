/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-11 13:51:48
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 17:25:46
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/src/server/usermodel.cpp
 * @Descripttion: 
 */
#include "usermodel.hpp"
#include "db_zsq.h"
#include <iostream>
using namespace std;

// User表的增加方法
bool UserModel::insert(User &user) {
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());

    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            // 获取插入成功的用户数据生成的主键id
            user.setId(mysql_insert_id(mysql.getConnection())); // mysql_insert_id是一个全局方法
            return true;
        }
    }

    return false;
}

// 根据用户号码查询用户信息
User UserModel::query(int id) {
    
    // 原本想这样User* UserModel::query(int id) {}，可以返回空指针，表示没查到，但是这就要释放资源了，又没有用智能指针

    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES *res = mysql.query(sql); // mysql查询操作，入口参数是sql语句

        if (res != nullptr) {
            MYSQL_ROW row = mysql_fetch_row(res); // 使用 mysql_fetch_row遍历结果集中的每一行
            
            if (row != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res); // 释放资源 没有这一行的话，内存会不断泄漏
                return user;
            }
        }
    }

    return User();
}

// 更新用户的状态信息
bool UserModel::updateState(User user) {
    // 1.组装sql语句
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", 
            user.getState().c_str(), user.getId());

    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            return true;
        }
    }
    return false; // 出作用域，mysql对象会自动调用析构函数释放资源
}

// 重置用户的状态信息
void UserModel::resetState() {
    // 1.组装sql语句
    char sql[1024] = "update user set state = 'offline' where state = 'online'";

    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}