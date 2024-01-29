#include <iostream>

using namespace std;

template <typename T1>
class A {
public:
    template <typename T2>
    A(T2 v1, T2 v2); /* 构造函数模板，引入了自己的模板参数 T2 */

    A(double v1, double v2)
    {
        cout << "A<T1>::A(double, double) 执行了" << endl;
    }

    A(T1 v1, T1 v2) /* 不是构造函数模板，而是构造函数 */
    {
        cout << "A<T1>::A(T1, T1) 执行了" << endl;
    }

    template <typename T3> /* 普通成员函数 */
    void myft(T3 tmpt)
    {
        cout << tmpt << endl;
    }

    T1 m_ic;
    static constexpr int m_stcvluae = 200;
};

/* 类外实现类模板的构造函数 */
template <typename T1>
template <typename T2>
A<T1>::A(T2 v1, T2 v2)
{
    cout << "A<T1>::A(T2 v1, T2 v2) 执行了!" << endl;
}

int main(void)
{
    A<float> a1(1, 2); /* 函数是可以推断的 */

    // A<float> a2(1.1, 2.2); /* myft<double> */

    A<float> a2(1.1, 2.2); /* A::A(double, double) */

    // A<float> a3(1.1f, 2.2f); /* A::A(T2, T2) */

    A<float> a3(1.1f, 2.2f); /* A::A(T1, T1) */
}