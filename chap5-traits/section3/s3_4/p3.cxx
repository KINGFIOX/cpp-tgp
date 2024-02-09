#include <iostream>
#include <type_traits>
#include <utility>

class PAR { };

class SON : public PAR { };

class SON_o : private PAR { };

class OTHER { };

void f(PAR*)
{
    std::cout << "f(PAR*) 执行了" << std::endl;
}

void f(void*)
{
    std::cout << "f(void*) 执行了" << std::endl;
}

int main(void)
{
    f(static_cast<PAR*>(nullptr)); /* f(PAR*) */
    f(static_cast<SON*>(nullptr)); /* f(PAR*) */
    // f(static_cast<SON_o*>(nullptr)); /* 错误： Cannot cast 'SON_o' to its private base class 'PAR' */
    f(static_cast<OTHER*>(nullptr)); /* f(void*) */
}
