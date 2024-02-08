#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

int main(void)
{
    std::cout << IsSame<int, int>::value << std::endl; // 1
    std::cout << IsSame<int, float>::value << std::endl; // 0
}