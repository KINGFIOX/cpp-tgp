#include <algorithm>
#include <iostream>

using namespace std;

template <typename T>
class myTestC {
public:
    /* 这个不是万能引用，因为不能类型推断 */
    void testfunc(T&& x) { }

public:
    /* 这个成员函数的 x 形参就是：万能引用类型 */
    template <typename T2>
    void testfunc2(T2&& x) { }
};

int main(void)
{
    myTestC<int> mc;
    int i = 100;
    // mc.testfunc(i); /* 错误： Rvalue reference to type 'int' cannot bind to lvalue of type 'int' */
    mc.testfunc(std::move(i)); /* ✅ */

    mc.testfunc2(i); /* ✅ */
    mc.testfunc2(100); /* ✅ */
}