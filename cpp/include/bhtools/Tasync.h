
#ifndef TASYNC_H
#define TASYNC_H

#include <iostream>
#include <functional>
#include <list>
#include <cassert>

#include "Tany.h"

namespace bhtools {


// 异步回调函数顺序执行-优化地狱回调问题-可选择性接受运算结果的返回值
template <typename Tval = nullptr_t>
struct Tasync
{
    Tasync() {}
    ~Tasync() { quit_th(); }

    // 首先任务
    template <typename Tret>
    Tasync& start(std::function<Tret()> fn = nullptr)
    {
        if(fn != nullptr)
        { 
            _fn_start = [=]() -> Tret {
                Tret ret;
                ret = fn();
                return ret;
            };
        }
        return *this;
    }

    // 继续任务
    template <typename Tret,typename Targ>
    Tasync& next(std::function<Tret(Targ)> fn = nullptr)
    {
        if(fn != nullptr)
        {
            _ls_next.push_back([=](Tany arg) -> Tret {
                Tret ret;
                ret = fn(arg.value<Targ>());
                return ret;
            });
        }
        return *this;
    };

    // 结束任务
    template <typename Targ>
    void end(std::function<Tval(Targ)> fn = nullptr)
    { 
        if(fn != nullptr)
        {
            _fn_end = [=](Tany any) -> Tval {
                Tval ret;
                ret = fn(any.value<Targ>());
                _suc = true;
                return ret;
            };
        }
    };

    // 设置下一个任务的参数-通过该入口穿参-不推荐使用
    template <typename Targ>
    void set_next_arg(Targ arg)
    { _arg = arg; }

    // 设置错误时回调
    void set_error_func(std::function<void(const std::string &)> fn = nullptr)
    { _fn_err = fn;  };

    // 停止运行-传入错误日志
    void stop_run(const std::string &err)
    {
        _run = false; 
        _suc = false;
        if(_fn_err)
        { _fn_err(err); }
    }

    // 执行任务
    void exec_task()
    {
        if(_run && _fn_start)
        { _arg = _fn_start(); }
        for(const auto &fn:_ls_next)
        { if(_run) { _arg = fn(_arg); } }
        if(_run && _fn_end)
        { _val = _fn_end(_arg); }
        _run = false;
    }

    // 退出线程并释放资源
    void quit_th()
    {
        if(_th != nullptr) 
        { 
            _th->join(); 
            delete _th;
            _th = nullptr;
        }
    }

    // 任务启动入口-默认多线程异步执行-重复执行会阻塞线程
    void run(bool async = true)
    {
        _run = true;
        if(async)
        {
            quit_th();
            _th = new std::thread(&Tasync::exec_task,this);
        }
        else { exec_task(); }
    }

    // 返回结果与状态
    Tval get_value() { return _val; }
    bool is_suc() { return _suc; }
    bool is_run() { return _run; }


    bool _suc = false;              // 运算结果是否成功
    bool _run = false;              // 继续执行标记
    Tval _val;                      // 运行结果值
    Tany _arg = nullptr;            // 下一个执行函数的参数
    std::thread *_th = nullptr;     // 进入子线程执行
    std::function<void(const std::string &)> _fn_err = nullptr;     // 触发错误回调
    std::function<Tany()> _fn_start = nullptr;                      // 开始回调-计算参数
    std::function<Tval(Tany)> _fn_end = nullptr;                    // 结束回调-计算结果
    std::list<std::function<Tany(Tany)>> _ls_next;                  // 列表回调-运算中间过程
};


} // bhtools




#endif // TASYNC_H
