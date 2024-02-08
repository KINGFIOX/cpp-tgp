#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

#if 0

template <typename T>
// struct voidValueTraits : std::false_type { };
struct voidValueTraits {
    static const bool value = false;
};

template <>
// struct voidValueTraits<void> : std::true_type {
struct voidValueTraits<void> {
    static const bool value = true;
};

#else

template <typename T>
struct voidValueTraits : std::false_type { };

template <>
struct voidValueTraits<void> : std::true_type { };

#endif

int main(void)
{
    // std::cout << std::is_void<int>::value << std::endl; // 0
    // std::cout << std::is_void<void>::value << std::endl; // 1

    std::cout << voidValueTraits<int>::value << std::endl; // 0
    std::cout << voidValueTraits<void>::value << std::endl; // 1
}