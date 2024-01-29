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

    template <typename T3, typename T4>
    void myft(T3 tmpt1, T4 tmpt2)
    {
        cout << "myft(T3, T4) 泛化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }

    template <typename T4>
    void myft(int tmpt1, T4 tmpt2);
    // {
    //     cout << "myft(int, T4) 偏特化版本" << endl;
    //     cout << tmpt1 << endl;
    //     cout << tmpt2 << endl;
    // }

    template <>
    void myft(int tmpt1, float tmpt2);
    // {
    //     cout << "myft(int, float) 全特化版本" << endl;
    //     cout << tmpt1 << endl;
    //     cout << tmpt2 << endl;
    // }
};

/* 类外实现 */
template <typename T1>
template <typename T4>
void A<T1>::myft(int tmpt1, T4 tmpt2)
{
    cout << "myft(int, T4) 偏特化版本" << endl;
    cout << tmpt1 << endl;
    cout << tmpt2 << endl;
}

// template <typename T1>
// /* 错误： Out-of-line definition of 'myft' does not match any declaration in 'A<T1>' */
// void A<T1>::myft(int tmpt1, float tmpt2)
// {
//     cout << "myft(int, float) 全特化版本" << endl;
//     cout << tmpt1 << endl;
//     cout << tmpt2 << endl;
// }

template <>
class A<float> {
public:
    A(float v1, float v2) /* 不是构造函数模板，而是构造函数 */
    {
        cout << "A<T1>::A(T1, T1) 执行了" << endl;
    }

    template <typename T3, typename T4>
    void myft(T3 tmpt1, T4 tmpt2)
    {
        cout << "A<char*>::myft(int, float) 泛化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }

    /* 可声明可不声明 */
    // template <>
    // void myft(int tmpt1, float tmpt2);

    template <typename U>
    class OtherC {
    public:
        void myfOC()
        {
            cout << "myfOC 执行了" << endl;
        }
    };
};

// /* A<float>中有泛化版本的 myft，因此不用在 A<float> 中声明如下的全特化版本了 */
// template <>
// void A<float>::myft(int tmpt1, float tmpt2)
// {
//     cout << "myft(int, float) 全特化版本" << endl;
//     cout << tmpt1 << endl;
//     cout << tmpt2 << endl;
// }

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.myft(3, 3.5f); // myft(int, float) 全特化版本
}
