#include <iostream>
#include <type_traits>
#include <utility>

/* ---------- ---------- is Default constructible ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U = std::void_t<>>
class IsDefaultConstructible : public std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T>
class IsDefaultConstructible<T, std::void_t<decltype(T())>> : public std::true_type { };

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

class B {
public:
    B(int tmpValue)
    {
    }
};

int main(void)
{
    std::cout << IsDefaultConstructible<A>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<B>::value << std::endl; // 0
}
