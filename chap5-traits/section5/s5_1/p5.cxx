/**
 * @file p3.cxx
 * @author your name (you@domain.com)
 * @brief 使用 SumPolicy、MinPolicy 策略类，但是初值可能有问题
 * @version 0.1
 * @date 2024-02-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <climits>
#include <cstdint>
#include <iostream>

/* ---------- ---------- 之前写的 固定萃取 ---------- ---------- */

template <typename T>
struct SumFixedTraits;

template <>
struct SumFixedTraits<char> {
    using sumT = int;
    static sumT initValue() { return 0; }
};

template <>
struct SumFixedTraits<int> {
    using sumT = int64_t;
    static sumT initValue() { return 0; }
};

template <>
struct SumFixedTraits<double> {
    using sumT = double;
    static sumT initValue() { return 0.0; }
};

template <typename T>
struct MinFixedTraits;

template <>
struct MinFixedTraits<char> {
    using sumT = int;
    static sumT initValue() { return CHAR_MAX; }
};

template <>
struct MinFixedTraits<int> {
    using sumT = int64_t;
    static sumT initValue() { return INT_MAX; }
};

template <>
struct MinFixedTraits<double> {
    using sumT = double;
    static sumT initValue() { return 1.0 / 0.0; }
};

/* ---------- ---------- 策略 ---------- ---------- */

template <typename sumT, typename T>
struct SumPolicy {
    static void algorithm(sumT& sum, const T& value) /* 策略类的核心算法 */
    {
        sum += value; // 求和
    }
};

template <typename sumT, typename T>
struct MinPolicy {
    static void algorithm(sumT& min, const T& value) /* 策略类的核心算法 */
    {
        min = min > value ? value : min;
    }
};

/* ---------- ---------- funcSum ---------- ---------- */

// template <typename T>
// auto funcSum(const T* begin, const T* end)
// {
//     using sumT = typename SumFixedTraits<T>::sumT;
//     sumT sum = SumFixedTraits<T>::initValue();

//     for (;;) {
//         sum += (*begin);
//         if (begin == end) {
//             break;
//         }
//         ++begin;
//     }

//     return sum;
// }

template <typename T,
    typename U = SumFixedTraits<T>,
    template <class, class> class V = SumPolicy // 模板模板参数
    >
auto funcSum(const T* begin, const T* end)
{
    typename U::sumT result = U::initValue();

    for (;;) {
        V<typename U::sumT, T>::algorithm(result, *begin);
        // V::algorithm(result, *begin);
        // T 是 数组成员类型，U 是 固定萃取 类模板，
        // 从中可以提取出 结算的结果类型（U::sumT）以及结果的初值
        // V 是策略类模板，用于实现具体算法（求和，求最小值）
        if (begin == end) {
            break;
        }
        ++begin;
    }

    return result;
}

int main(void)
{
    char arr1[] = "abc";
    std::cout << (int)funcSum(&arr1[0], &arr1[2]) << std::endl;

    std::cout << funcSum<char, SumFixedTraits<int>>(&arr1[0], &arr1[2]) << std::endl;
    /* 这个版本的 SumFixedTraits 的 sumT 就是 int64_t */

    int arr2[] = { 2, 5, 10, 1, 3 };
    std::cout << funcSum<char, SumFixedTraits<int>, MinPolicy>(&arr1[0], &arr1[4]) << std::endl;
    /* 然而这里得到的是 0，这显然是不正确的，为什么会不正确？因为这里初值设置的有问题，一开始就是 0，已经很小了 */
}