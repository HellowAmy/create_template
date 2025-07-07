
#ifndef FARGV_H
#define FARGV_H

#include <iostream>
#include <vector>
#include "Fstm.h"


namespace bhtools {


// 解析命令行传入参数
struct Fargv
{
    Fargv() {}
    ~Fargv() {}

    // 初始化参数列表-常规参数
    void init(int argc,char **argv)
    {
        for(int i=0;i<argc;i++)
        {
            _vec.push_back(argv[i]);
        }
    }

    // 初始化参数列表-分析字符串
    void init(const std::string &args)
    {
        bool split = true;
        char csplit = ' ';
        std::string val;
        for(const char a:args)
        {
            if(split && a == ' ')
            {
                if(val.size() != 0)
                { 
                    _vec.push_back(val);
                    val.clear();
                }
            }
            else if(a == '\'' || a == '\"') 
            { 
                if(csplit == ' ')
                {
                    csplit = a;
                    split = !split; 
                }
                else if(a == csplit)
                {
                    csplit == ' ';
                    split = !split; 
                }
                else 
                { val.push_back(a); }
            } 
            else 
            { val.push_back(a); }
        }
        if(val.size() != 0)
        {
            _vec.push_back(val);
            val.clear();
        }
    }

    // 判断参数存在
    bool is_exist(const std::string &arg)
    {
        for(auto &a:_vec)
        {
            if(a == arg)
            { return true; }
        }
        return false;
    }

    // 存在参数时返回对应值
    std::string is_exist_arg(const std::string &arg,const std::string &end = "-")
    {
        bool find = false;
        for(int i=0;i<_vec.size();i++)
        {
            if(find)
            {
                if(is_exist_sub(_vec[i],end) == false)
                { return _vec[i]; }
                break;
            }

            if(find == false && _vec[i] == arg)
            { find = true; }
        }
        return "";
    }

    // 存在参数时返回对应参数列表
    std::vector<std::string> is_exist_args(const std::string &arg,const std::string &end = "-")
    {
        bool find = false;
        std::vector<std::string> vec_args;
        for(int i=0;i<_vec.size();i++)
        {
            if(find)
            {
                if(is_exist_sub(_vec[i],end))
                { return vec_args; }
                else 
                { vec_args.push_back(_vec[i]); }
            }

            if(find == false && _vec[i] == arg)
            { find = true; }
        }
        return vec_args;
    }

    // 存在参数时返回参数赋值的路径
    std::string is_exist_path(const std::string &arg,const std::string &end = "=")
    {
        for(int i=0;i<_vec.size();i++)
        {
            if(is_exist_sub(_vec[i],arg))
            { return bhtools::Fstm(_vec[i])(end,1,1); }
        }
        return "";
    }

    // 判断字符串是否存在子串
    inline static bool is_exist_sub(const std::string &str,const std::string &sub)
    {
        auto tup = Ffinds::find_sub(str,sub);
        return std::get<0>(tup);
    }


    std::vector<std::string> _vec;      // 存储所有传入的参数
};


} // bhtools

#endif // FARGV_H
