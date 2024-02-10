#include <iostream>

template <int n_v>
struct Factorial {
    enum {
        value = n_v * Factorial<n_v - 1>::value
    };
};

/**
 * @brief 递归 base case
 *
 * @tparam
 */
template <>
struct Factorial<1> {
    enum {
        value = 1
    };
};

int main(void)
{
    constexpr int var = Factorial<5>::value;
    static_assert(var == 120, "factorial error");
    std::cout << var << std::endl;
}