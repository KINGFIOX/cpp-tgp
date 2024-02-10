#include <iostream>

constexpr int myfunc(int abc)
{
    return abc * 2;
}

int main(void)
{
    int i = 64;
    constexpr int var = 11 * myfunc(12);
    /* 静态断言，编译期间断言 */
    static_assert(var == 264, "std error");

    return 0;
}