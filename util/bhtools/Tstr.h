
#ifndef TSTR_H
#define TSTR_H

#include <string>
#include <tuple>
#include <sstream>
#include <iomanip>

namespace bhtools {


// 基础通用字符串处理
struct Tstr
{
    // 各类型转字符串函数
    inline static std::string to_string(bool val) 
    {
        if(val) { return "true"; } 
        return "false"; 
    }

    inline static std::string to_string(char val) 
    {
        std::string ret = "['";
        ret.push_back(val);
        ret += "': ";
        ret += Tstr::to_string((int)val);
        ret += "]";
        return ret;
    }

    inline static std::string to_string(const char *val) 
    {
        if(val) { return val; } 
        return "NULL"; 
    }

    inline static std::string to_string(const std::string &val) 
    { return val; }

    template<typename T>
    inline static std::string to_string(const T &val) 
    { return std::to_string(val); }

    template<typename T>
    inline static std::string to_string(T* val) 
    { std::stringstream ss; ss<<val; return ss.str(); }

    template<typename T1,typename T2>
    inline static std::string to_string(const std::pair<T1,T2> &pair) 
    { return "["+Tstr::to_string(pair.first)+" : "+Tstr::to_string(pair.second)+"]"; }

    template<typename ...T>
    inline static std::string to_string(const std::tuple<T...> &tup) 
    { 
        std::string ret;
        Tstr_tup<std::tuple<T...>,std::tuple_size<std::tuple<T...>>::value,0>::action(tup,ret); 
        return ret;
    }

    // 多参数-浮点数
    inline static std::string to_string(double val,int dec) 
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(dec) << val;
        return ss.str();
    }


    // 字符串转各类型-可查看是否成功
    template<typename T> inline static 
    bool from_string(const std::string &str,T &ret)
    {
        bool suc = true;
        try {
            if(std::is_same<T,double>::value) 
            { ret = std::stod(str); }
            else if(std::is_same<T,float>::value) 
            { ret = std::stof(str); }
            else if(std::is_same<T,int>::value || std::is_same<T,unsigned int>::value) 
            { ret = std::stoi(str); }
            else if(std::is_same<T,long>::value || std::is_same<T,unsigned long>::value) 
            { ret = std::stol(str); }
            else if(std::is_same<T,long long>::value || std::is_same<T,unsigned long long>::value) 
            { ret = std::stoll(str); }
            else { suc = false; }
        } catch(...) { suc = false; }
        return suc;
    }

    // 字符串转各类型-不在意成功
    template<typename T> inline static 
    T from_string(const std::string &str)
    { T ret; std::istringstream ss(str); ss>>ret; return ret; }


    // 发现子串位置
    // 传参数 < 字符串 - 子串 - 偏移位置 >
    // 返回值 < 是否成功 - 成功的下标 >
    inline static std::tuple<bool,size_t> 
    find_sub(const std::string &str,const std::string &sub,size_t offset = 0)
    {
        auto it = str.find(sub,offset);
        if(it != std::string::npos)
        { return std::make_tuple(true,it); }
        return std::make_tuple(false,0);
    }

    // 发现子串范围位置-标记位置是最开始和最末尾
    // 传参数 < 字符串 - 开始标记 - 末尾标记 - 偏移位置 >
    // 返回值 < 是否成功 - 开始标记的首位 - 末尾标记的末尾 - 标记中间的字符串 >
    inline static std::tuple<bool,size_t,size_t,std::string> 
    find_range(const std::string &str,const std::string &fb,const std::string &fe,size_t offset = 0)
    {
        auto rb = find_sub(str,fb,offset); 
        auto re = find_sub(str,fe,offset); 
        if(std::get<0>(rb) && std::get<0>(re))
        { 
            size_t ib = std::get<1>(rb);
            size_t ie = std::get<1>(re);
            return std::make_tuple(true,ib,ie + fe.size() -1,section_range(str,ib + fb.size(),ie)); 
        }
        return std::make_tuple(false,0,0,"");
    }

    // 切割字符串-传入开始和末尾下标-获取两个下标间的字符串-开区间和闭区间
    // 传参数 < 字符串 - 开区间 - 闭区间 >
    // 返回值 < 分割的字符串 >
    inline static std::string section_range(const std::string &str,size_t ib,size_t ie)
    {
        std::string ret;
        if(ie > ib && ie <= str.size())
        {
            for(size_t i=ib;i<ie;i++)
            {
                ret.push_back(str[i]);
            }
        }
        return ret;
    }


    // internal
    template<typename T> inline static 
    std::string from_stringstream(const T &val)
    { std::stringstream ss; ss<<val; return ss.str(); }

    
    // 中途循环模板
    template<typename Tclass, size_t count,size_t now>
    struct Tstr_tup
    {
        static void action(Tclass obj,std::string &str) 
        {
            if(now != (count - 1))
            {
                auto val = std::get<now>(obj);
                str += Tstr::to_string(val) + " : ";
            }
            Tstr_tup<Tclass,count,now+1>::action(obj,str);
        }
    };

    // 退出模板
    template<typename Tclass,size_t count>
    struct Tstr_tup<Tclass,count,count>
    {
        static void action(Tclass obj,std::string &str) 
        {
            if(count != 1)
            {
                auto val = std::get<count-1>(obj);
                str += Tstr::to_string(val) + "] ";
            }
        }
    };

    // 首次进入模板
    template<typename Tclass,size_t count>
    struct Tstr_tup<Tclass,count,0>
    {
        static void action(Tclass obj,std::string &str) 
        {
            auto val = std::get<0>(obj);
            if(count != 1)
            { str += "[" + Tstr::to_string(val) + " : "; }
            else 
            { str += "[" + Tstr::to_string(val) + "] "; }

            Tstr_tup<Tclass,count,1>::action(obj,str);
        }
    };
};


} // bhtools

#endif // TSTR_H
