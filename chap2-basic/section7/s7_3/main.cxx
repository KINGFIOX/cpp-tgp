#include <iostream>

using namespace std;

#include "ca.hxx"

/* 实例化定义，一般只有一个.cpp文件里这么写，编译器为其生成代码。在其他的 .cpp文件中，可以extern template A<float> */
template class A<float>;

/* 函数模板实例化定义 */
template void myfunc(int& v1, int& v2);

int main(void)
{
    A<float> a1(1, 2);
    a1.myft(3); //

    A<float> a2(1, 2);
    a2.myft(3); //
}