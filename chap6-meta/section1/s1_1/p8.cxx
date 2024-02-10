#include <iostream>

/**
 * @brief 变量模板，泛化版本
 *
 * @tparam Arg
 */
template <int Arg>
constexpr int result()
{
    return Arg * result<Arg - 1>();
}

/**
 * @brief 变量模板，特化版本
 *
 * @tparam
 */
template <>
constexpr int result<0>()
{
    return 1;
}

int main(void)
{
    std::cout << result<5>() << std::endl;
}