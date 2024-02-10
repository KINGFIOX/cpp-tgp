#include <iostream>
#include <type_traits>

template <int x_v, int y_v>
struct InstantiationObServe {
    using TmpType = typename std::conditional<(x_v > y_v),
        InstantiationObServe<x_v - 1, y_v>, InstantiationObServe<x_v, y_v - 1>>::type;

    // static const int value = TmpType::value;
    enum { value = TmpType::value };
};

template <int x_v>
struct InstantiationObServe<x_v, x_v> {
    // static const int value = x_v;
    enum { value = x_v };
};

int main(void)
{
    std::cout << InstantiationObServe<6, 4>::value << std::endl; // 4
}