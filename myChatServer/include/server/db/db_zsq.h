/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-11 11:42:24
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-11 11:54:30
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/db/db_zsq.h
 * @Descripttion: 
 */
#ifndef DB_ZSQ_H
#define DB_ZSQ_H

#include <mysql/mysql.h>
#include <string>
using namespace std;

// 数据库操作类
class MySQL
{
public:
    // 初始化数据库连接
    MySQL();
    // 释放数据库连接资源
    ~MySQL();
    // 连接数据库
    bool connect();
    // 更新操作
    bool update(string sql);
    // 查询操作
    MYSQL_RES *query(string sql);
    // 获取连接
    MYSQL* getConnection();
private:
    MYSQL *_conn;
};

#endif