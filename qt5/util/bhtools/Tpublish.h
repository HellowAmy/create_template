
#ifndef TPUBLISH_H
#define TPUBLISH_H

#include <iostream>
#include <map>
#include <set>
#include <functional>
#include <list>
#include <typeindex>

#include "Tany.h"

namespace bhtools {


// 消息转发基础类-新消息发布时通知基础类-由基础类中转到具体数据消息类
struct Tpubmsg_base
{
    // 将发布消息转发到具体类型的子类
    void notify_any(const Tany &any)
    {
        for(auto &a:_notify_arr)
        {   
            if(a.second) { a.second(any); } 
        }
    }

    std::map<std::type_index,std::function<void(const Tany &)>> _notify_arr;    // 存储不同数据类型的子类
};


// 发布类-支持多个消息通知到多位订阅者
struct Tpublish
{
    // 插入订阅者到对应消息类型
    template<typename Tmsg>
    void insert(Tpubmsg_base *user)
    { 
        auto &it = _sub[std::type_index(typeid(Tmsg))];
        it.insert(user);
    }

    // 移除对应消息的订阅者
    template<typename Tmsg>
    void remove(Tpubmsg_base *user)
    {
        auto &it = _sub[std::type_index(typeid(Tmsg))];
        it.erase(user);
    }

    // 删除订阅者所有注册消息
    void remove(Tpubmsg_base *user)
    {
        for(auto &a:_sub)
        {
            auto &set = _sub[a.first];
            auto it = set.find(user);
            if(it != set.end()) { set.erase(user); }
        }
    }

    // 检查并释放无订阅者的消息
    void release_overdue()
    {
        std::vector<std::type_index> vec;
        for(auto &a:_sub)
        {
            auto &set = _sub[a.first];
            if(set.size() == 0) { vec.push_back(a.first); }
        }
        for(auto &a:vec)
        { _sub.erase(a); }
    }

    // 查找是否存在注册
    template<typename Tmsg>
    bool find(Tpubmsg_base *user)
    {
        auto &it = _sub[std::type_index(typeid(Tmsg))];

        if(it.find(user) != it.end()) { return true; }
        return false;
    }

    // 通知该类型的所有订阅者
    template<typename Tmsg>
    void notify(const Tmsg &msg)
    {
        auto it = _sub.find(std::type_index(typeid(msg)));
        if(it != _sub.end())
        {
            for(auto &user:it->second)
            { user->notify_any(Tany(msg)); }
        }
    }

    std::map<std::type_index,std::set<Tpubmsg_base *>> _sub;    // 存储不同消息类型的多个订阅者
};


// 具体消息类-当使用者继承时可订阅发布的内容
template<typename Tmsg>
struct Tpubmsg : virtual Tpubmsg_base
{
    Tpubmsg() { init(); }

    // 构造时传入发布者
    Tpubmsg(Tpublish *pub)
    {
        init();
        install_pub(pub);
    }

    // 将具体消息类注册到消息基础类中-接收到消息发布时转为具体数据类型
    void init()
    {
        _notify_arr[std::type_index(typeid(Tmsg))] = [=](Tany any) {
            if(any.use<Tmsg>())
            { notify_msg(any.value<Tmsg>()); }
        };
    }

    // 安装消息发布者
    void install_pub(Tpublish *pub)
    { if(pub) { pub->insert<Tmsg>(this); }  }

    // 接受具体消息数据类型
    virtual void notify_msg(const Tmsg &msg) = 0;
};





} // bhtools

#endif // TPUBLISH_H
