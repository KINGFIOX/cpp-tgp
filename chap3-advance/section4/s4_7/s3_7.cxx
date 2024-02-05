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

int main(void)
{
    myfunc(15); /* myfunc 普通函数执行了 */
    /* 开发者的直觉：如果 函数模板 与 普通函数 都满足，感觉上是优先使用 普通函数 */
}