/**
 * @file s3_4.cxx
 * @author your name (you@domain.com)
 * @brief 非 void_t 不可
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <type_traits>
#include <utility>

using namespace std;

struct NoInnerType {
    /* 没有类型别名 */
    int m_i;
};

struct HaveInnerType {
    using type = int; /* 类型别名 */
    void myfunc() { }
};

/* 泛化版本 */
// template <typename T, typename U = std::void_t<>>
template <typename T, typename U = int>
struct HasMemFunc : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>> : std::true_type { };

int main(void)
{
    cout << HasMemFunc<NoInnerType>::value << endl; // 0 ---(int)---> 0
    cout << HasMemFunc<HaveInnerType>::value << endl; // 1 ---(int)---> 0
}