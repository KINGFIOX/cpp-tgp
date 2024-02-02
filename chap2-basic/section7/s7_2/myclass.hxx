#ifndef __MYCLASS_HXX__
#define __MYCLASS_HXX__

#include <iostream>

// class MyClass {
// public:
//     void func();
// };

template <typename T>
class MyClass {
public:
    void func();
};

template <typename T>
void MyClass<T>::func()
{
    std::cout << "MyClass::func 成员函数执行了!" << std::endl;
}

/**
 * @brief 偏特化
 *
 */
template <>
class MyClass<int> {
public:
    void func();
};

void MyClass<int>::func()
{
    std::cout << "MyClass::func 成员函数执行了!" << std::endl;
}

#endif