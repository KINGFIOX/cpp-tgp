/**
 * @file s5_4.cxx public MyClassT<Args, Args...>... 继承
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

using namespace std;

template <typename... Args>
class MyClassT {
public:
    MyClassT()
    {
        cout << "MyClassT::MyClassT() 执行了，可变参数个数=" << sizeof...(Args) << endl;
    }
};

template <typename... Args>
class MyClassT2 : public MyClassT<Args, Args...>... {
public:
    MyClassT2()
    {
        cout << "MyClassT2::MyClassT2() 执行了，可变参数个数=" << sizeof...(Args) << endl;
    }
};

int main(void)
{
    MyClassT2<float, int, double> tmpobj;
    /**
     * MyClassT::MyClassT() 执行了，可变参数个数=4
     * MyClassT::MyClassT() 执行了，可变参数个数=4
     * MyClassT::MyClassT() 执行了，可变参数个数=4
     * MyClassT2::MyClassT2() 执行了，可变参数个数=3
     */
}

#ifdef INSIGHTS_USE_TEMPLATE
template <>
class MyClassT<float, float, int, double> {

public:
    inline MyClassT() { }
};

template <>
class MyClassT<int, float, int, double> {

public:
    inline MyClassT() { }
};

template <>
class MyClassT<double, float, int, double> {

public:
    inline MyClassT() { }
};

template <>
class MyClassT2<float, int, double>
    : public MyClassT<float, float, int, double>,
      public MyClassT<int, float, int, double>,
      public MyClassT<double, float, int, double> {

public:
    inline MyClassT2()
        : MyClassT<float, float, int, double>()
        , MyClassT<int, float, int, double>()
        , MyClassT<double, float, int, double>()
    {
    }
};

add_rvalue_reference()

#endif