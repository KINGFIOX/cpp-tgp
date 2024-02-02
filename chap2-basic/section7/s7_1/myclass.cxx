#include <iostream>

#include "myclass.hxx"

using namespace std;

template <typename T>
void MyClass<T>::func()
{
    std::cout << "MyClass::func 成员函数执行了!" << endl;
}