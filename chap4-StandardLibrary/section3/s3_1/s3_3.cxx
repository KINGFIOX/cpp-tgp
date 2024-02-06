/**
 * @file s3_3.cxx
 * @author your name (you@domain.com)
 * @brief 判断是否有 成员hanshu
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
template <typename T, typename U = std::void_t<>>
struct HasMemFunc : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>> : std::true_type { };
/**
 * 语义上，理解为是创建了一个 临时对象
 */

int main(void)
{
    cout << HasMemFunc<HaveInnerType>::value << endl;
}