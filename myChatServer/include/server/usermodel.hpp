/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-11 13:47:33
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-11 14:50:55
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/usermodel.hpp
 * @Descripttion: 
 */

// 提供相应的数据库操作方法
// 专门操作model的类
// 剥离业务层和数据层中间耦合的代码

#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"

// User表的数据操作类 针对表的操作无非就是增删改查
class UserModel {
public:
    // User表的增加方法
    bool insert(User &user);

    // 根据用户号码查询用户信息
    User query(int id);

    // 更新用户的状态信息
    bool updateState(User user);

    // 重置用户的状态信息
    void resetState();
};

#endif