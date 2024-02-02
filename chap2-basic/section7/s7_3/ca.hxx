#ifndef __CA_HXX__
#define __CA_HXX__

#include <iostream>

template <typename C>
class A {
public:
    template <typename T2>
    A(T2 v1, T2 v2);

    template <typename T>
    void myft(T tmpt)
    {
        std::cout << tmpt << std::endl;
    }

    C m_ic;
};

template <typename C>
template <typename T2>
A<C>::A(T2 v1, T2 v2)
{
    std::cout << v1 << v2 << std::endl;
}

template <typename T>
void myfunc(T v1, T v2)
{
    std::cout << v1 + v2 << std::endl;
}

#endif