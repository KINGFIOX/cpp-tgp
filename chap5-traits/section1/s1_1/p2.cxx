#include <cstdint>
#include <iostream>

using namespace std;

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
};

template <>
struct SumFixedTraits<int> {
    using sumT = int64_t; // 代表返回的类型是 int 类型
};

// ...(其他)给进来的是某一个类型，返回的是另一种类型，可以任意扩展出多个 SumFixedTraits 类模板的特化版本

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;
    sumT sum {};
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
    int arr1[] = { 10, 15, 20 };
    int arr2[] = { 1000000000, 1500000000, 2000000000 };
    char arr3[] = "abc";

    cout << funcSum(&arr1[0], &arr1[2]) << endl;
    cout << funcSum(&arr2[0], &arr2[2]) << endl;
    cout << funcSum(&arr3[0], &arr3[2]) << endl;
}