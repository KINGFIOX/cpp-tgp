#include <iostream>

/**
 * @brief 泛化版本。
 * 		这个比较有意思，这个 泛化版本 也是 base case，相较于上面
 * @tparam FTArgs
 */
template <int... FTArgs>
constexpr int resultSum = 0;

/**
 * @brief 特化版本
 *
 * @tparam First
 * @tparam Others
 */
template <int First, int... Others>
constexpr int resultSum<First, Others...> = First + resultSum<Others...>;

int main(void)
{
    std::cout << resultSum<1, 10, 100> << std::endl;
}