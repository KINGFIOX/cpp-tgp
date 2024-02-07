#include <iostream>
#include <type_traits>

struct A {
};

union B {
};

template <class T, T val>
struct IntegralConstant {
    static const T value = val;
    using value_type = T;
    using type = IntegralConstant;
    inline constexpr operator value_type() const noexcept { return value; }
    inline constexpr value_type operator()() const _NOEXCEPT { return value; }
};

int main(void)
{
    std::cout << std::is_union<A>::value << std::endl; // 0
    std::cout << std::is_union_v<A> << std::endl;
    std::cout << std::is_union<B>::value << std::endl; // 1

    std::cout << std::is_class_v<A> << std::endl; // 1
    std::cout << std::is_class_v<B> << std::endl; // 0

    // std::integral_constant<bool, std::is_union<B>::value> myObj1;
    IntegralConstant<bool, std::is_union<B>::value> myObj1;
    std::cout << myObj1.value << std::endl; // 1

    std::integral_constant<bool, std::is_union<A>::value> myObj2;
    std::cout << myObj2.value << std::endl; // 0
}