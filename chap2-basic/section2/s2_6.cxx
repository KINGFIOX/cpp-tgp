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

    static int m_stc; /* 声明一个静态成员变量 */
};

template <typename T, typename U>
int TC<T, U>::m_stc = 50; /* 定义一个静态成员变量 */

template <>
struct TC<int, int> {
    static int m_stc;
};

int TC<int, int>::m_stc = 100;

int main(void)
{
    TC<double, float> df;
    cout << df.m_stc << endl; // 50

    TC<int, int> ii;
    cout << ii.m_stc << endl; // 100
}