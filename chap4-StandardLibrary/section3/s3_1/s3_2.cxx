/**
 * @file s3_2.cxx 判断是否有 成员变量
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <type_traits>

using namespace std;

struct NoInnerType {
    /* 没有类型别名 */
    int m_i;
};

struct HaveInnerType {
    using type = int; /* 类型别名 */
    void myfunc() { }
};

#define HAS_TYPE_MEM(parMTpNm)                        \
    template <typename T, typename U = std::void_t<>> \
    struct HTM_##parMTpNm : std::false_type { };      \
    template <typename T>                             \
    struct HTM_##parMTpNm<T, std::void_t<typename T::parMTpNm>> : std::true_type { };
/* ##parMTpNm 这里的 ##parMTpNm 解引用 + 字符串连接 */

/* 使用宏定义：HTM_type */
HAS_TYPE_MEM(type);

/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct HasMember : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMember<T, std::void_t<decltype(T::m_i)>> : std::true_type { };
/**
 *  如果不包含 m_i 成员，那么 decltype(T::m_i) 就有错误，
 * 然后又根据 SFINAE 特性，编译器就不会选择特化版本，而是选择泛化版本
 */

int main(void)
{
    cout << HTM_type<HaveInnerType>::value << endl;
}