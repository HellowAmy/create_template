
#ifndef TANY_H
#define TANY_H

#include <iostream>
#include <typeindex>
#include <memory>

namespace bhtools {


// 存储任何值-使用无类型指针存储数据
struct Tany
{
    // 类型保存-保存的基类指针-用于类型擦除
    struct type_base
    {
        virtual ~type_base() {}
        virtual type_base* get() = 0;
        virtual type_base* copy() = 0;
    };

    // 类型保存-存储具体值得模板类-用于生成指针
    template<typename T>
    struct type_pack : public type_base
    {
        type_pack(const T &val) : _val(val) { }

        // 返回自身
        inline type_base* get() override { return this; }

        // 将值内容分配到指针
        inline type_base* copy() override { return new type_pack<T>(_val); }

        T _val;     // 动态保存的值
    };


    // 默认无数据
    Tany() : _type(&typeid(void)) {}

    // 有数据
    template<typename T>
    Tany(const T &val) 
    { set(val); }

    Tany(const char *val) 
    { set(val); }

    // 拷贝构造只复制保存值
    Tany(const Tany &any)
    { set(any); }

    // 重载赋值
    template<typename T>
    Tany& operator=(const T &val)
    { set(val); return *this; }

    Tany& operator=(const char *val)
    { set(val); return *this; }

    Tany& operator=(const Tany &any)
    { set(any); return *this; }

    // 释放保存值
    ~Tany() { reset(); }

    // 查询使用的类型
    template<typename T>
    bool use() { return (*_type == typeid(T)); }

    // 直接返回保存值-需要先查询
    template<typename T>
    const T& value() { return (static_cast<type_pack<T>*>(_val))->_val; }


    // internal
    // 构造传入值时创建新的保存值指针
    template<typename T>
    void set(const T &val)
    {
        _type = &typeid(T);
        _val = new type_pack<T>(val);
    }

    // 释放当前保存指针并拷贝传入保存值
    void set(const Tany &any)
    {
        if(_val) { reset(); }
        _type = any._type;
        _val = any.copy();
    }

    // 重置状态和释放保存值
    void reset()
    {
        _type = &typeid(void);
        if(_val) { delete _val; _val = nullptr; }
    }
 
    // 用于拷贝构造复制对象保存值
    type_base* copy() const
    { 
        if(_val) { return _val->copy(); }
        return nullptr;
    }

    const std::type_info *_type = nullptr;  // 保存值得类型
    type_base *_val = nullptr;              // 保存值的无类型指针
};


} // bhtools

#endif // TANY_H
