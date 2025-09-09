
#ifndef TUNPACK_H
#define TUNPACK_H

#include <functional>
#include <mutex>
#include <vector>

#include "Tbyte.h"

namespace bhtools {


// 拆包参数-分隔符
struct Tunfd_delimit
{
    const std::string _delimit = "\n";
};


// 拆包函数-分隔符
template<typename T = Tunfd_delimit>
struct Tunf_delimit
{
    // 传入数据返回包体
    static std::string pack(const std::string &ctx)
    {
        T arg; 
        return ctx + arg._delimit;
    }

    // 拆包函数
    std::vector<std::string> unpack(const std::string &ctx,bool ths)
    {
        size_t sidel = _arg._delimit.size();
        std::vector<std::string> vec;

        if(sidel == 0) { return vec; }
        if(ths) { _mux.lock(); }

        std::string all = _save + ctx;
        size_t go = _save.size();

        // 循环拆包
        while(go < all.size())
        {
            // 检查到分隔符
            if(all[go] == _arg._delimit[0])
            {
                if((go + sidel) > all.size()) { break; }

                // 检测分割符
                bool into = true;
                for(size_t i=0;i<sidel;i++)
                {
                    if(all[go + i] != _arg._delimit[i])
                    { 
                        into = false;
                        go += i;
                        break; 
                    }
                }

                // 匹配成功
                if(into)
                {
                    std::string data = std::string(all.begin(),all.begin() + go);
                    vec.push_back(data);

                    all = std::string(all.begin() + go + sidel,all.end());
                    go = 0;
                }
            }
            go++;
        }
        _save = all;

        if(ths) { _mux.unlock(); }
        return vec;
    }

    T _arg;             // 拆包函数参数
    std::mutex _mux;    // 线程锁
    std::string _save;  // 保存上次数据
};
 

// 拆包函数-包长度
template<typename T = size_t>
struct Tunf_length
{
    // 传入数据返回包体
    static std::string pack(const std::string &ctx)
    {
        T arg;
        arg = ctx.size();
        arg = Tendian::to_net(arg);
        return std::string((char*)&arg,sizeof(arg)) + ctx;
    }

    // 拆包函数
    std::vector<std::string> unpack(const std::string &ctx,bool ths)
    {
        if(ths) { _mux.lock(); }

        std::vector<std::string> vec;
        std::string all = _save + ctx;
        size_t len_head =  sizeof(_arg);

        while(true)
        {
            // 包头字节不足退出
            if(all.size() < len_head) { break; }

            // 数据长度不足退出
            size_t len_data = all.size() - len_head;
            std::string head = std::string(all.begin(),all.begin() + len_head);
            _arg = *(decltype(_arg)*)head.c_str();
            _arg = Tendian::to_host(_arg);

            if(len_data < _arg) { break; }
            
            // 数据达到或超出包范围-回调包体数据
            std::string data = std::string(all.begin() + len_head,all.begin() + len_head + _arg);
            vec.push_back(data);
            
            all = std::string(all.begin() + len_head + _arg,all.end());
        }
        _save = all;

        if(ths) { _mux.unlock(); }
        return vec;
    }

    T _arg;             // 拆包函数参数
    std::mutex _mux;    // 线程锁    
    std::string _save;  // 保存上次数据
};

// 
// 
// 
// 

// 数据拆包组合
template<typename Tfn>
struct Tunpack
{
    // 拆包函数
    std::vector<std::string> unpack(const std::string &ctx,bool ths = true)
    { return _fn.unpack(ctx,ths); }

    // 组包函数
    inline static std::string pack(const std::string &ctx)
    { return Tfn::pack(ctx); }

    Tfn _fn;     // 拆包类型
};

//
//
//
//

// 定义快捷使用类型
using Tunpack_len = Tunpack<Tunf_length<>>;
using Tunpack_del = Tunpack<Tunf_delimit<>>;


} // bhtools

#endif // TUNPACK_H
