#include <algorithm>
#include <iostream>

using namespace std;

template <typename T>
class myTestC {
public:
    /* 这个不是万能引用，因为不能类型推断 */
    void testfunc(T&& x) { }
};

int main(void)
{
    myTestC<int> mc;
    int i = 100;
    // mc.testfunc(i); /* 错误： Rvalue reference to type 'int' cannot bind to lvalue of type 'int' */
    mc.testfunc(std::move(i)); /* ✅ */
}