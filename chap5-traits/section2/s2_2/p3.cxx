#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

#if 1

/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
struct IsSame : std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam T
 */
template <typename T>
struct IsSame<T, T> : std::true_type { };

#else

/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
struct IsSame {
    static const bool value = false;
};

/**
 * @brief 特化版本
 *
 * @tparam T
 */
template <typename T>
struct IsSame<T, T> {
    static const bool value = true;
};

#endif

int main(void)
{
    std::cout << IsSame<int, int>::value << std::endl; // 1
    std::cout << IsSame<int, float>::value << std::endl; // 0
}