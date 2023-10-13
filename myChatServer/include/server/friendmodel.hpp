/*
 * @version: 
 * @Author: zsq 1363759476@qq.com
 * @Date: 2023-10-12 21:26:27
 * @LastEditors: zsq 1363759476@qq.com
 * @LastEditTime: 2023-10-12 21:31:33
 * @FilePath: /Linux_nc/ChatServer_2023/myChatServer/include/server/friendmoel.hpp
 * @Descripttion: 
 */
#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

#include "user.hpp"
#include <vector>
using namespace std;

// 维护好友信息的操作接口方法
class FriendModel
{
public:
    // 添加好友关系
    void insert(int userid, int friendid);

    // 返回用户好友列表
    vector<User> query(int userid);

    // 对于c和c++来说，做业务可能没python还有java做业务那么方便
    // 所以业务方面就不做过多的扩展了，我艹，这样的嘛，果然得学别的语言
    // 另外，好友列表一般是记录在客户端的，因为一般好友信息比较多，要是登录成功都是通过网络从服务器返回给客户端的话，服务器压力太大了
};

#endif