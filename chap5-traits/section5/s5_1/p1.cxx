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

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;
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