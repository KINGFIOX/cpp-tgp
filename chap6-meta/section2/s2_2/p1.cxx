#include <iostream>

/**
 * @brief 泛化版本
 *
 * @tparam x_v
 * @tparam y_v
 */
template <int x_v, int y_v>
struct InstantiationObServe {
    static const int value = (x_v > y_v) ? InstantiationObServe<x_v - 1, y_v>::value
                                         : InstantiationObServe<x_v, x_v>::value;
};

/**
 * @brief 特化版本
 *
 * @tparam x_v
 */
template <int x_v>
struct InstantiationObServe<x_v, x_v> {
    static const int value = x_v;
};

int main(void)
{
    std::cout << InstantiationObServe<6, 4>::value << std::endl; // 4
}