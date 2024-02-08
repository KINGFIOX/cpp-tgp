#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

class A {
public:
    A(int v1, int v2)
        : m_i(v1 + v2)
    {
    }
    int m_i;
};

/* ---------- ---------- 固定萃取 ---------- ---------- */

/**
 * @brief 泛化版本，只声明不实现
 *
 * @tparam T
 */
template <typename T>
struct SumFixedTraits;

/**
 * @brief 特化版本
 *
 * @tparam
 */
template <>
struct SumFixedTraits<char> {
    using sumT = int; // 代表返回的类型是 int 类型
    static const sumT initValue = 0; // static const 是在 编译期间 就确定好的
};

template <>
struct SumFixedTraits<int> {
    using sumT = int64_t; // 代表返回的类型是 int 类型
    static const sumT initValue = 0;
};

// FIXME
template <>
struct SumFixedTraits<A> {
    using sumT = A; // 代表返回的类型是 int 类型
    // static const sumT initValue { 0, 0 };
    static const sumT initValue;
    /**
     * 错误： Static data member of type 'const sumT' (aka 'const A') must be initialized out of line
     *
     * 带有累内初始化表达式的 静态数据成员 必须具有不可变的常量整型类型，或必须被指定为 内联
     */
    // inline static const sumT initValue { 0, 0 }; // 也是可以的 （C++17 以后）
};
A const SumFixedTraits<A>::initValue = A { 0, 0 };

// FIXME
template <>
struct SumFixedTraits<double> {
    using sumT = double; // 代表返回的类型是 int 类型
    // static const sumT initValue = 0.0;
    /**
     * 错误： in-class initializer for static data member of type 'const sumT' (aka 'const double')
     * 		requires 'constexpr' specifier [-Wstatic-float-init]
     *
     * MSVC： 带有累内初始化表达式的 静态数据成员 必须具有不可变的常量整型类型，或必须被指定为 内联
     */

    // 但是如果使用 C++11 以后的 constexpr 就可以了
    static constexpr sumT initValue = 0.0;

    // inline static constexpr sumT initValue = 0.0; // 同样也可以
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;

    // sumT sum {};
    sumT sum = SumFixedTraits<T>::initValue; // 给进来一个类型 T，返回一个值。

    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    double arr4[] = { 1.8, 2.3, 3.7 };

    cout << funcSum(&arr4[0], &arr4[2]) << endl;
}