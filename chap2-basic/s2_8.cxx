/**
 * @file s2_8.cxx 范围偏特化
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

    void functest()
    {
        cout << "functest1 泛化版本" << endl;
    }

    static int m_stc; /* 声明一个静态成员变量 */
};

template <typename T, typename U>
struct TC<const T, U*> {
    TC()
    {
        cout << "TC<const T, U*> 偏特化版本构造函数" << endl;
    }

    void functest();
};

template <typename T, typename U>
void TC<const T, U*>::functest()
{
    cout << "TC<const T, U*>::functest 偏特化版本" << endl;
}

int main(void)
{
    TC<const int, int*> t; // TC<const T, U*> 偏特化版本构造函数
    t.functest(); // TC<const T, U*>::functest 偏特化版本
    return 0;
}