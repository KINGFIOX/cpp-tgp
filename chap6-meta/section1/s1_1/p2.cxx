#include <iostream>

template <int x_v, int y_v>
struct Calc {
    /* 这是一个匿名的枚举类型 */ /* 把枚举类型值当做常量 */
    enum {
        addValue = x_v + y_v,
        subValue = x_v - y_v,
    };
};

int main(void)
{
    const int var3 = Calc<12, 15>::addValue;
    const int var4 = Calc<12, 15>::subValue;
    static_assert(var3 == 27, "calc error");

    std::cout << var3 << std::endl;
    std::cout << var4 << std::endl;
}