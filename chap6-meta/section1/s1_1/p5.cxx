#include <iostream>

constexpr int Factorial(int n_v)
{
    return n_v <= 1 ? 1 : (n_v * Factorial(n_v - 1));
}

int main(void)
{
    constexpr int var = Factorial(5);
    static_assert(var == 120, "factorial error");
    std::cout << var << std::endl;
}