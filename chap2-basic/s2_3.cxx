/**
 * @file s2_3.cxx 自定义推断指南
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

using namespace std;

template <typename T>
struct A {
    T m_b;
};
template <typename T>
A(T) -> A<double>;

int main(void)
{
    A<int> aObj1; /* 需要明确指定模板参数类型 */

    A<int> aObj2 { 15 }; /* 需要明确的指定 模板参数类型 */
    A aObj3 { 15 };
    cout << aObj3.m_b << endl;
    cout << typeid(aObj3.m_b).name() << endl; // d
}