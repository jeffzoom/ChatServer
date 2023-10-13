/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-11 11:44:33
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-11 23:02:28
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/src/server/db/db_zsq.cpp
 * @Descripttion: 
 */
#include "db_zsq.h" // db_zsq.h，原本的名字是db.h，但是貌似和linux启动文件的/usr/include/db.h重名了，所以我就改了一个名字
#include <muduo/base/Logging.h>

// 数据库配置信息
static string server = "127.0.0.1";
// static string user = "root";
// static string password = "123456";
static string user = "zsq";
static string password = "*Zz123456";
static string dbname = "chat";

// 这六个函数是直接拷贝下来的

// 初始化数据库连接   其实是开辟了一块存储连接资源的连接数据的一块资源空间
MySQL::MySQL() {
    _conn = mysql_init(nullptr);
}

// 释放数据库连接资源
MySQL::~MySQL() {
    if (_conn != nullptr)
        mysql_close(_conn);
}

// 连接数据库
bool MySQL::connect() {
    MYSQL *p = mysql_real_connect(_conn, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr) {
        // C和C++代码默认的编码字符是ASCII，如果不设置，从MySQL上拉下来的中文会显示问号（？）
        mysql_query(_conn, "set names gbk"); // 让代码能够支持中文
        LOG_INFO << "connect mysql success!";
    } else {
        LOG_INFO << "connect mysql fail!";
    }

    return p;
}

// 更新操作
bool MySQL::update(string sql) {
    if (mysql_query(_conn, sql.c_str())) {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }

    return true;
}

// 查询操作
MYSQL_RES *MySQL::query(string sql) {
    if (mysql_query(_conn, sql.c_str())) {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    
    return mysql_use_result(_conn);
}

// 获取连接
MYSQL* MySQL::getConnection() {
    return _conn;
}
