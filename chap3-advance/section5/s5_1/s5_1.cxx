/**
 * @file s5_1.cxx MyClassT<Args...> 继承
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
class MyClassT2 : public MyClassT<Args...> {
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
     * MyClassT::MyClassT() 执行了，可变参数个数=3
     * MyClassT2::MyClassT2() 执行了，可变参数个数=3
     */
}