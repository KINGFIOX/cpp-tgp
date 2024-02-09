/**
 * @file p2.cxx
 * @author your name (you@domain.com)
 * @brief 模板参数
 * @version 0.1
 * @date 2024-02-10
 *
 * @copyright Copyright (c) 2024
 *
 */

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

template <typename T, typename U = SumFixedTraits<T>>
auto funcSum(const T* begin, const T* end)
{
    typename U::sumT sum = U::initValue();

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
    char arr[] = "abc";
    std::cout << (int)funcSum(&arr[0], &arr[2]) << std::endl;

    std::cout << funcSum<char, SumFixedTraits<int>>(&arr[0], &arr[2]) << std::endl;
    /* 这个版本的 SumFixedTraits 的 sumT 就是 int64_t */
}