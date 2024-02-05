#include <iostream>

using namespace std;

template <typename T>
void myfunc(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc(int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

template <typename T>
void myfunc1(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc1(unsigned int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

template <typename T>
void myfunc2(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc2(unsigned short int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

int main(void)
{

    myfunc(15); /* myfunc 普通函数执行了 */
    /* 开发者的直觉：如果 函数模板 与 普通函数 都满足，感觉上是优先使用 普通函数 */

    myfunc1(15); /* myfunc 函数模板执行了 */
    /* 15，编译器一般认为是 signed int。如果调用 普通函数，要隐式转换为 unsigned int，
    但是如果直接套用模板，就不用转换，编译器觉得 模板更加合适 */

    myfunc1(15U); /* myfunc 普通函数执行了 */

    myfunc2(15); /* 函数模板执行了 */
}