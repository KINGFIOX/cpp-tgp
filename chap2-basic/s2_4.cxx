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

/**
 * @brief TC 的全特化版本
 *
 * @tparam
 */
template <>
struct TC<int, int> {
    TC()
    {
        cout << "TC<int, int> 特化版本 构造函数" << endl;
    }

    void functest1();

    void functest2()
    {
    }
};

void TC<int, int>::functest1()
{
    cout << "TC<int, int>::functest1" << endl;
}

int main(void)
{
    TC<int, float> myTC;
    TC<int, int> mytc2;
    // mytc2.functest1(); /* 错误： No member named 'functest1' in 'TC<int, int>' */
}