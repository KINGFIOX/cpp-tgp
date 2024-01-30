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

template <typename U>
struct TC<float, U> {
    TC()
    {
        cout << "TC<float, U> 偏特化版本构造函数" << endl;
    }

    void functest();
};

template <typename U>
void TC<float, U>::functest()
{
    cout << "TC<float, U>::functest() 偏特化版本" << endl;
}

int main(void)
{
    TC<float, int> myTc4;
    myTc4.functest();

    return 0;
}