#include <iostream>

using namespace std;

template <typename T1>
class A {
public:
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

public:
    /* 拷贝构造函数模板 */
    template <typename U>
    A(A<U>& other)
    {
        cout << "A::A(const A<U>& other) 拷贝构造函数模板执行了!" << endl;
    }

    /* 真正的 拷贝构造函数 */
    A(const A& other)
    {
        cout << "A<T1>::A(const A<T1>& other) 拷贝构造函数执行了" << endl;
    }

    /* 拷贝构造赋值 模板 */
    template <typename U>
    A<T1>& operator=(const A<U>& other)
    {
        cout << "A::operator=(const A<U>& other) 拷贝赋值运算符模板执行了 !" << endl;
        return *this;
    }
};

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.m_ic = 3.14;
    A<float> a4(a3); /* A<T1>::A(const A<T1>& other) 拷贝构造函数执行了 */
    cout << a4.m_ic << endl; // 3.14
}
