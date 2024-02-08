#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

class A {
public:
    A(int v1, int v2)
        : m_i(v1 + v2)
    {
    }

public:
    A& operator+=(const A& obj)
    {
        this->m_i += obj.m_i;
        return *this;
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
    // static const sumT initValue = 0; // static const 是在 编译期间 就确定好的
    static sumT initValue() { return 0; }
};

template <>
struct SumFixedTraits<int> {
    using sumT = int64_t; // 代表返回的类型是 int 类型
    static sumT initValue() { return 0; }
};

// FIXME
template <>
struct SumFixedTraits<A> {
    using sumT = A;
    static sumT initValue()
    {
        return sumT { 0, 0 };
    }
};

// FIXME
template <>
struct SumFixedTraits<double> {
    using sumT = double;
    static sumT initValue() { return 0; }
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;

    // sumT sum {};
    // sumT sum = SumFixedTraits<T>::initValue; // 给进来一个类型 T，返回一个值。
    sumT sum = SumFixedTraits<T>::initValue();

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
    A myAObj[] = { A { 2, 3 }, A { 3, 4 }, A { 5, 6 } };
    cout << funcSum(&arr4[0], &arr4[2]) << endl;
    cout << funcSum(&myAObj[0], &myAObj[2]).m_i << endl;
}