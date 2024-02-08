#include <cstdint>
#include <iostream>

using namespace std;

/**
 * @brief 计算和值的 函数模板
 */
template <typename U, typename T>
U funcSum(const T* begin, const T* end)
{
    U sum {};
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

    cout << funcSum<int64_t>(&arr1[0], &arr1[2]) << endl;
    cout << funcSum<int64_t>(&arr2[0], &arr2[2]) << endl;
    cout << funcSum<int64_t>(&arr3[0], &arr3[2]) << endl;
}