
#ifndef TASYNC_H
#define TASYNC_H

#include <iostream>
#include <functional>
#include <list>
#include <cassert>

namespace bhtools {


// 异步回调函数顺序执行-优化地狱回调问题-需要将所有参数聚合传入
template <typename Targs = void>
struct Tasync
{
    Tasync() {}
    
    ~Tasync() { quit_th(); }

    // 复制任务列表
    Tasync(const Tasync &other) 
    {  
        if(this != &other)
        {
            _fn_succee = other._fn_succee;
            _fn_error = other._fn_error;
            _ls_next = other._ls_next;
        }
    }

    // 开始任务
    Tasync& next(std::function<bool(Targs *)> fn = nullptr)
    {
        if(fn) { _ls_next.push_back(fn); }
        return *this;
    };

    // 完成任务回调
    Tasync& succee(std::function<void(Targs *)> fn = nullptr)
    {
        _fn_succee = fn;
        return *this;
    };

    // 完成任务回调
    Tasync& error(std::function<void(Targs *)> fn = nullptr)
    {
        _fn_error = fn;
        return *this;
    };

    // 任务启动入口-默认多线程异步执行-重复执行会阻塞线程
    bool run(Targs *any = nullptr,bool async = true)
    {
        if(std::is_same<Targs, void>::value == false && any == nullptr)
        { return false; }

        // 多次进入时需要等待上次运行完毕
        if(async)
        {
            _arg = any;
            quit_th();
            _th = std::make_shared<std::thread>(&Tasync::exec_task,this);
        }
        else 
        { 
            _arg = any;
            exec_task(); 
        }
        return true;
    }

    // 是否成功
    bool is_suc() { return _suc; }

    // 是否正在运行
    bool is_run() { return _run; }


    // internal
    // 执行任务
    void exec_task()
    {
        _run = true;
        _suc = true;
        for(const auto &fn:_ls_next)
        { 
            if(_suc && (fn(_arg) == false))
            { _suc = false;  }
        }
        _run = false;

        if(_fn_succee && _suc) { _fn_succee(_arg); }
        else if(_fn_error) { _fn_error(_arg); }
    }

    // 退出线程并释放资源
    void quit_th()
    { if(_th) { _th->join(); } }

    bool _suc = false;                                      // 运算结果是否成功
    bool _run = false;                                      // 继续执行标记
    Targs *_arg = nullptr;                                  // 执行任务参数
    std::shared_ptr<std::thread> _th;                       // 执行任务线程
    std::function<void(Targs *)> _fn_succee = nullptr;      // 完成列表后回调
    std::function<void(Targs *)> _fn_error = nullptr;       // 无法完成任务的错误回调
    std::list<std::function<bool(Targs *)>> _ls_next;       // 任务列表
};


} // bhtools

#endif // TASYNC_H
