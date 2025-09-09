
#ifndef TLOG_H
#define TLOG_H

#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <thread>
#include <queue>
#include <mutex>
#include <memory>

#include "Tstr.h"
#include "Ftime.h"
#include "Tsin.h"
#include "Fstm.h"

namespace bhtools {


// 不同等级日志
namespace bhenum {
    
    enum class level
    {
        e_tra,
        e_deb,
        e_inf,
        e_war,
        e_err,
        e_fat,
    };
}

//
//
//
//

// 日志缓冲区
struct Tlog_buf 
{ 
    template<typename T>
    inline void push(const T &val) 
    { _str += Tstr::to_string(val); }

    inline void clear() 
    { _str.clear(); }

    inline std::string value() const
    { return _str; }

    std::string _str;   // 缓存内容
};


// 日志结束类标记
struct Tlog_end { };


// 日志基本类-用于创建各种输出类型的模板-不提供打印只提供写入与缓存
template<typename Tbuf,typename Tend,typename Tout>
struct Tlog_base
{
    using level = bhenum::level;

    // 设置日志等级-不显示低等级内容
    inline void set_level(const level &el) { _el = el; } 


    // internal
    // 传入等级
    inline Tlog_base& operator<<(const level &el)
    { 
        if(el >= _el) { _pass = true; }
        else { _pass = false; }
        return *this; 
    }

    // 打印内容
    inline Tlog_base& operator<<(Tend)
    { 
        if(_pass)
        { 
            _out.out(_buf);
            _buf.clear(); 
            _pass = false;
        } 
        return *this; 
    }

    // 传入换行
    template<typename T>
    inline Tlog_base& operator<<(const T &log)
    { 
        if(_pass) { _buf.push(log); } 
        return *this; 
    }

    bool _pass = false;         // 日志等级是否通过
    level _el = level::e_tra;   // 日志等级划分枚举
    Tbuf _buf;                  // 日志写入缓存内容
    Tout _out;                  // 日志输出类
};

//
//
//
//

// 空打印-用于测试 << 操作的耗时
struct Tlog_out_null 
{
    void out(const Tlog_buf &buf) { }    
};


// 命令行打印
struct Tlog_out_cmd 
{
    void out(const Tlog_buf &buf) { std::cout<<buf.value()<<std::endl; }    
};


// 文件打印
// 打印一千万行时换行符比换行函数速度提升近一倍
// 但换行符是可能会导致无法及时刷新缓冲区丢失数据
struct Tlog_out_file
{   
    using pair_name = std::pair<std::string,std::string>;

    void out(const Tlog_buf &buf) 
    { 
        if(_fs.is_open() == false) { _fs.open(_file,_mode); }
        if(_fs.is_open()) { _fs<<buf.value()<<"\n"; update_file(); }
    } 

    //初始化日志
    inline void reopen(const std::string &file,bool app = true)
    {
        _pname = get_pname(file);
        _file = file;
        _limit_now = 1;

        if(app) { _mode = std::ios::app; } 
        else { _mode = std::ios::out; } 

        if(_fs.is_open()) { _fs.close(); }
    }
    
    // 设置循环最大文件数-默认无限
    inline void set_limit(size_t max) { _limit_max = max; } 

    // 设置单个文件最大长度-默认64M
    inline void set_length(size_t len) { _len_max = len; }  

    // 设置每行写入时是否刷新文件
    inline void set_flush(bool flush) { _flush = flush; }  

    // 判断文件是否存在
    static bool exist_file(const std::string &filename)
    { std::ifstream f(filename); return f.is_open(); }


    // internal
    // 超出最大文件限制后更新文件名
    bool update_file() 
    {
        if(_flush) { _fs.flush(); }
        if(_len_max < _fs.tellg())
        {
            if(_limit_max == 0) { return write_unlimited(); }
            else { return write_limit(); } 
        }
        return true;
    }

    // 无限制日志
    bool write_unlimited()
    {
        _fs.close();
        for(int i=_limit_now;;i++)
        {
            std::string file = newfile(i);
            if(exist_file(file) == false)
            { 
                rename(_file.c_str(),file.c_str()); 
                _limit_now++;
                break;
            }
        }
        _fs.open(_file,_mode);
        return _fs.is_open();
    }

    // 限制日志数量
    bool write_limit() 
    {
        _fs.close();
        rename(_file.c_str(),newfile(_limit_now).c_str());
        _limit_now++;
        if(_limit_now > _limit_max) { _limit_now = 1; }
        
        _fs.open(_file,_mode);
        return _fs.is_open();
    }

    // 得到文件与后缀信息
    pair_name get_pname(const std::string &file)
    {
        std::string name = Fstm(file)(".",0,0);
        std::string suffix = Fstm(file)(".",1,1);
        return std::make_pair(name,suffix);
    }

    // 根据数量生成文件名
    std::string newfile(size_t num)
    {
        std::string file;
        file += _pname.first;
        file += "_" + std::to_string(num) + ".";
        file += _pname.second;
        return file;
    }

    bool _flush = true;             // 刷入缓冲区
    size_t _limit_max = 0;          // 日志文件限制数量
    size_t _limit_now = 1;          // 当前写入日志
    size_t _len_max = (1 << 26);    // 最大长度--64M
    pair_name _pname;               // 文件名与后缀
    std::string _file;              // 文件名
    std::fstream _fs;               // 文件对象
    std::ios_base::openmode _mode;  // 文件打开模式
};


// 异步文件日志
// 比对普遍文件日志在一千万输出时有十分之一的速度提升
// 程序退出时可能遗留日志需要手动刷入文件
template<typename Tbuf,size_t Ttime = 1000>
struct Tlog_out_asyn : public Tlog_out_file
{
    Tlog_out_asyn() {}

    ~Tlog_out_asyn() { exit_th(); }

    void out(const Tlog_buf &buf) { push_queue(buf); } 
    
    // 重启写入线程
    void reset()
    {
        if(_run == false)
        { 
            _run = true;
            _th = std::make_shared<std::thread>(&Tlog_out_asyn::work_write,this); 
        }
    }

    // 退出写入线程
    void exit_th()
    { 
        if(_run)
        {
            _run = false; 
            _th->join(); 
            Tlog_out_file::_fs.flush(); 
        }
    }


    // internal
    // 将队列内日志写入日志输出类
    void work_write()
    {
        while(_run)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(Ttime));
            std::unique_lock<std::mutex> lock(_mut);
            while (_que.empty() == false)
            {
                Tlog_out_file::out(_que.front());
                _que.pop(); 
            }
        }
    }

    // 加入到队列
    inline void push_queue(const Tbuf &txt)
    { 
        if(_run == false) { reset(); }
        std::unique_lock<std::mutex> lock(_mut); 
        _que.push(txt); 
    } 

    bool _run = false;                  // 写入线程运行标记
    std::mutex _mut;                    // 队列锁
    std::queue<Tbuf> _que;              // 写入日志队列
    std::shared_ptr<std::thread> _th;   // 写入线程
};


// 空打印日志
struct Tlog_null : public Tlog_base <Tlog_buf,Tlog_end,Tlog_out_null> 
{
    Tlog_null() { set_level(bhenum::level::e_tra); }
};


// 命令行打印日志
struct Tlog_cmd : public Tlog_base <Tlog_buf,Tlog_end,Tlog_out_cmd> 
{
    Tlog_cmd() { set_level(bhenum::level::e_tra); }
};


// 文件打印日志
struct Tlog_file : public Tlog_base <Tlog_buf,Tlog_end,Tlog_out_file> 
{
    Tlog_file(const std::string &file = "Tflog.log") 
    {
        set_level(bhenum::level::e_tra);
        _out.reopen(file); 
        _out.set_flush(true);
    }
};

// 文件打印日志-异步
struct Tlog_afile : public Tlog_base <Tlog_buf,Tlog_end,Tlog_out_asyn<Tlog_buf,500>> 
{
    Tlog_afile(const std::string &file = "Taflog.log") 
    {
        set_level(bhenum::level::e_tra);
        _out.reopen(file); 
        _out.set_flush(true);
    }
};

//
//
//
//

// 打印固定格式的时间
struct Tlog_time 
{
    inline static std::string print()
    {
        Ftimes::data d = Ftimes::to_data(Ftimes::time_now());
        d.hou += 8;
        return Ftimes::format_time(d,"%4Y-%2M-%2D.%2H:%2T:%2S.%3L");
    };
};


// 标准容器打印
struct Tlog_con
{
    template<typename T>
    inline static std::string print(const T& con,size_t len = 1,const std::string &flg = " ",const std::string &prev = "| ")
    {
        std::string ret = "\n";
        ret += prev + "size: " + std::to_string(con.size());
        ret += "\n" + prev;
        size_t count = 0;
        for(const auto &a:con)
        {
            if(len != 0 && count >= len) { count = 0; ret += "\n" + prev; }
            ret += Tstr::to_string(a) + flg;
            count++;
        }
        ret += "\n";
        return ret;
    }
};

//
//
//
//

// 日志格式化内容
#define BHLOG_FORMAT(tips,el,...)                           \
    el<<tips "["<<__FILE__<<":<"<<__LINE__<<">] <<<< "      \
    <<__VA_ARGS__


// 格式化VSCode快捷导航格式
#define BHLOG_FORMAT_VSC(tips,el,...)                       \
    el<<tips "<<<< "                                        \
    <<__VA_ARGS__                                           \
    <<" >>>>["<<__FILE__<<":"<<__LINE__                     \
    <<"][" +bhtools::Tlog_time::print()+"] "                \


// 生成打印字符串
#define BHLOG_PRINT(out,tips,end,BHDF_FM,el,...)            \
    out<<BHDF_FM(tips,el,__VA_ARGS__)<<end                  \


// 生成快捷打印宏-带颜色
#define BHLOG_MAKE_COLD(out,end,...)                                        \
    BHLOG_PRINT(out,"\033[32m[Deb]","\033[0m"<<end,BHLOG_FORMAT_VSC,        \
    bhtools::bhenum::level::e_deb,__VA_ARGS__)                              \

#define BHLOG_MAKE_COLI(out,end,...)                                        \
    BHLOG_PRINT(out,"\033[36m[Inf]","\033[0m"<<end,BHLOG_FORMAT_VSC,        \
    bhtools::bhenum::level::e_inf,__VA_ARGS__)                              \

#define BHLOG_MAKE_COLW(out,end,...)                                        \
    BHLOG_PRINT(out,"\033[33m[War]","\033[0m"<<end,BHLOG_FORMAT_VSC,        \
    bhtools::bhenum::level::e_war,__VA_ARGS__)                              \

#define BHLOG_MAKE_COLE(out,end,...)                                        \
    BHLOG_PRINT(out,"\033[31m[Err]","\033[0m"<<end,BHLOG_FORMAT_VSC,        \
    bhtools::bhenum::level::e_err,__VA_ARGS__)                              \


// 生成快捷打印宏
#define BHLOG_MAKED(out,end,...)                                        \
    BHLOG_PRINT(out,"[Deb]",end,BHLOG_FORMAT_VSC,                       \
    bhtools::bhenum::level::e_deb,__VA_ARGS__)                          \
        
#define BHLOG_MAKEI(out,end,...)                                        \
    BHLOG_PRINT(out,"[Inf]",end,BHLOG_FORMAT_VSC,                       \
    bhtools::bhenum::level::e_inf,__VA_ARGS__)                          \
        
#define BHLOG_MAKEW(out,end,...)                                        \
    BHLOG_PRINT(out,"[War]",end,BHLOG_FORMAT_VSC,                       \
    bhtools::bhenum::level::e_war,__VA_ARGS__)                          \
        
#define BHLOG_MAKEE(out,end,...)                                        \
    BHLOG_PRINT(out,"[Err]",end,BHLOG_FORMAT_VSC,                       \
    bhtools::bhenum::level::e_err,__VA_ARGS__)                          \

//
//
//
//

// 快捷打印宏
#ifndef BHLOG_CLOSE_LOG

    // 设置默认打印宏
    #define BHLOG_CONF_DEF(type) bhtools::Tsin_log_conf::get()->type

    // 快捷命令行打印
    #ifndef BHLOG_CLOSE_COL
        #define vlogd(...) BHLOG_MAKE_COLD(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vlogi(...) BHLOG_MAKE_COLI(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vlogw(...) BHLOG_MAKE_COLW(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vloge(...) BHLOG_MAKE_COLE(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #else
        #define vlogd(...) BHLOG_MAKED(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vlogi(...) BHLOG_MAKEI(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vlogw(...) BHLOG_MAKEW(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
        #define vloge(...) BHLOG_MAKEE(BHLOG_CONF_DEF(_cmd),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #endif

    // 快捷文件打印
    #define flogd(...) BHLOG_MAKED(BHLOG_CONF_DEF(_file),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define flogi(...) BHLOG_MAKEI(BHLOG_CONF_DEF(_file),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define flogw(...) BHLOG_MAKEW(BHLOG_CONF_DEF(_file),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define floge(...) BHLOG_MAKEE(BHLOG_CONF_DEF(_file),BHLOG_CONF_DEF(_end),__VA_ARGS__)

    // 快捷文件打印
    #define alogd(...) BHLOG_MAKED(BHLOG_CONF_DEF(_afile),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define alogi(...) BHLOG_MAKEI(BHLOG_CONF_DEF(_afile),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define alogw(...) BHLOG_MAKEW(BHLOG_CONF_DEF(_afile),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define aloge(...) BHLOG_MAKEE(BHLOG_CONF_DEF(_afile),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    
    // 快捷空值打印
    #define nlogd(...) BHLOG_MAKED(BHLOG_CONF_DEF(_null),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define nlogi(...) BHLOG_MAKEI(BHLOG_CONF_DEF(_null),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define nlogw(...) BHLOG_MAKEW(BHLOG_CONF_DEF(_null),BHLOG_CONF_DEF(_end),__VA_ARGS__)
    #define nloge(...) BHLOG_MAKEE(BHLOG_CONF_DEF(_null),BHLOG_CONF_DEF(_end),__VA_ARGS__)

#else
    #define vlogd(...)
    #define vlogi(...)
    #define vlogw(...)
    #define vloge(...)

    #define flogd(...) 
    #define flogi(...) 
    #define flogw(...) 
    #define floge(...) 
#endif

// 默认打印宏
struct Tsin_log_conf
{
    Tlog_end _end;
    Tlog_cmd _cmd;
    Tlog_null _null;
    Tlog_file _file;
    Tlog_afile _afile;

    // 简化日志类函数调用
    template<typename T>
    void reopen(T &ptr,const std::string &file,bool app = true) 
    { ptr._out.reopen(file,app); }

    template<typename T>
    void set_level(T &ptr,const bhenum::level &el) 
    { ptr.set_level(el); }

    template<typename T>
    void set_length(T &ptr,size_t len) 
    { ptr._out.set_length(len); }

    template<typename T>
    void set_limit(T &ptr,size_t max) 
    { ptr._out.set_limit(max); }

    void exit_flush_afile() 
    { _afile._out.exit_th(); }

    static Tsin_log_conf* get() 
    { return Tsind<Tsin_log_conf>::get(); }
};

// 快捷打印操作
#define $(value) "["#value": "<<value<<"] "
#define $C(value) "["#value": "<<bhtools::Tlog_con::print(value)<<"] "
#define $S(value) "["#value": "<<bhtools::Tstr::from_stringstream(value)<<"] "


} // bhtools

#endif // TLOG_H
