#include <iostream>

/**
 * @brief 折叠表达式
 *
 * @tparam FTArgs
 * @return constexpr int
 */
template <int... FTArgs>
constexpr int resultSum()
{
    return (... + FTArgs);
}

int main(void)
{
    std::cout << resultSum<1, 10, 100>() << std::endl;
}