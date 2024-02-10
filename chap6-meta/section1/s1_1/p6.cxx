#include <iostream>

constexpr int Factorial(int n_v)
{
    int result = 1;
    for (int i = 1; i <= n_v; i++) {
        result *= i;
    }
    return result;
}

int main(void)
{
    constexpr int var = Factorial(5);
    static_assert(var == 120, "factorial error");
    std::cout << var << std::endl;
}