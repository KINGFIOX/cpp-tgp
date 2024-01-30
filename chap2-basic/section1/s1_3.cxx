/**
 * @file s3.cxx 全特化
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

template <typename T, typename U>
void tfunc(T&& tmprv1, U&& tmprv2)
{
    cout << "tfunc(T&&, U&&) 泛化版本" << endl;
    cout << tmprv1 << endl;
    cout << tmprv2 << endl;
}

template <>
void tfunc<int, double>(int& tmprv1, double& tmprv2)
{
    cout << "tfunc(int&, double&) 全特化版本" << endl;
    cout << tmprv1 << endl;
    cout << tmprv2 << endl;
}

int main(void)
{
    const char* p = "i love china!";
    int i = 12;
    tfunc(p, i); // void tfunc<const char*, int>(const char*& tmprv1, int& tmprv2)

    tfunc(1, 2.0); // 错误，没有匹配类型，因为这里是右值
    double d = 15.0;
    tfunc(i, d);
}