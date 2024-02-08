#include <iostream>

template <class T1, class T2>
void print_is_same()
{
    std::cout << typeid(T1).name() << std::endl;
}