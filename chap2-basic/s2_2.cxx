#include <iostream>

using namespace std;

template <typename T>
struct A {
    A(T val1, T val2)
    {
        cout << "A::A(T val1, T val2)执行了" << endl;
    }

    A(T val)
    {
        cout << "A::A(T val)执行了" << endl;
    }
};

int main(void)
{
    A aObj1(15, 16); /* int */

    A aOjb2(12.8); /* double */
}