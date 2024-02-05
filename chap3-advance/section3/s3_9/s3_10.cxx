#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
// typename std::enable_if<(sizeof(T) > 2)>::type funcEI()
std::enable_if_t<(sizeof(T) > 2)> funcEI()
/* 相当于是 void funcEI() */
{
}

int main(void)
{
    funcEI<char>(); /* 错误：no matching function for call to 'funcEI' */
}