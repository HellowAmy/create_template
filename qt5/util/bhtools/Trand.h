
#ifndef TRAND_H
#define TRAND_H

#include <random>

namespace bhtools {


// 随机种子生成
template<typename Tdev = std::random_device,typename Tgen = std::mt19937>
struct Trand_gen
{
    Trand_gen() { _gen.seed(_rd()); }

    Tdev _rd;
    Tgen _gen;
};


// 整数与浮点分布计算
template<typename Tdis ,typename T>
struct Trand_value : public Tdis, Trand_gen<>
{
    using Tdis::Tdis;
    T value() { return (*this)(_gen); }
};

//
//
//
//

// 快捷使用方式
using Trand_int = Trand_value<std::uniform_int_distribution<>,int>;             // 均匀-整数分布
using Trand_double = Trand_value<std::uniform_real_distribution<>,double>;      // 均匀-浮点分布
using Trand_bool = Trand_value<std::bernoulli_distribution,bool>;               // 均匀-布尔分布
using Trand_ndis = Trand_value<std::normal_distribution<>,double>;              // 正态-距离中间点扩散概率
using Trand_ddis = Trand_value<std::discrete_distribution<>,int>;               // 离散-列表中下标出现概率
using Trand_pdis = Trand_value<std::poisson_distribution<>,int>;                // 泊松-模拟一小时内发生次数分布
using Trand_gdis = Trand_value<std::gamma_distribution<>,double>;               // 泊松-模拟所有事件发生总长分布


} // bhtools

#endif // TRAND_H
