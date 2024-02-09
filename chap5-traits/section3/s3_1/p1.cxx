#include <iostream>
#include <type_traits>

class A { };

class B {
public:
    B(int tmpVal) { }
};

int main(void)
{
    std::cout << std::is_default_constructible<int>::value << std::endl; // 1
    std::cout << std::is_default_constructible<A>::value << std::endl; // 1
    std::cout << std::is_default_constructible<B>::value << std::endl; // 0
}