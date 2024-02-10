#include <iostream>

template <int x_v, int y_v>
struct Calc {
    static const int addValue = x_v + y_v;
    static const int subValue = x_v - y_v;
};
// 使用 枚举值就不会给 addValue 分配空间
// 但是 上面这种就是会给 addValue 分配实际的空间

int main(void)
{
    const int var3 = Calc<12, 15>::addValue;
    const int var4 = Calc<12, 15>::subValue;
    static_assert(var3 == 27, "calc error");

    std::cout << var3 << std::endl;
    std::cout << var4 << std::endl;
}