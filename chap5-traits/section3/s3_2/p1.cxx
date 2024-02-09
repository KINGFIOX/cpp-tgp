#include <iostream>
#include <type_traits>

class A { };

class B : public A { };

class C : private A { };

int main(void)
{
    std::cout << std::is_convertible<float, int>::value << std::endl; // 1
    std::cout << std::is_convertible<int, float>::value << std::endl; // 1

    std::cout << std::is_convertible<A, B>::value << std::endl; // 0 ： A ---> B 是不行了
    std::cout << std::is_convertible<B, A>::value << std::endl; // 1

    std::cout << std::is_convertible<C, A>::value << std::endl; // 0
}