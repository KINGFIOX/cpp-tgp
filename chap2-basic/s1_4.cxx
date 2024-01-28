/**
 * @file s4.cxx 偏特化
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

template <typename T, typename U>
void tfunc(T& tmprv1, U& tmprv2)
{
    cout << "tfunc(T&, U&) 泛化版本" << endl;
    cout << tmprv1 << endl;
    cout << tmprv2 << endl;
}

// template <typename U> 错误的
// void tfunc<double, U>(double& tmprv1, U& tmprv2) /* function template partial specialization is not allowed */
// {
//     cout << "tfunc(T&, U&) 泛化版本" << endl;
//     cout << tmprv1 << endl;
//     cout << tmprv2 << endl;
// }

/* 这种实际上是重载 */
template <typename T, typename U>
void tfunc(const T& tmpRv1, U& tmpRv2)
{
    cout << "tfunc(const T&, U&) 重载版本" << endl;
}

template <typename U>
void tfunc(double& tmprv1, U& tmprv2) /* function template partial specialization is not allowed */
{
    cout << "tfunc(T&, U&) 泛化版本" << endl;
    cout << tmprv1 << endl;
    cout << tmprv2 << endl;
}

int main(void)
{
    const char* p = "i love china!";
    int i = 12;
    tfunc(p, i); // void tfunc<const char*, int>(const char*& tmprv1, int& tmprv2)

    double d = 15.0;
    tfunc(i, d);
}