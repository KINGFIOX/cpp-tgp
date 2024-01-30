/**
 * @file s2_5.cxx 普通成员函数的 全特化
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
struct TC {
    TC()
    {
        cout << "TC 泛化版本 构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }
};

template <>
void TC<double, int>::functest1()
{
    cout << "普通成员函数 TC<double, int>::functest1 的全特化版本" << endl;
}

int main(void)
{
    TC<double, int> a;
    a.functest1(); // 普通成员函数 TC<double, int>::functest1 的全特化版本
}