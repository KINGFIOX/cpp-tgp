#include "myclass.hxx"

int main(void)
{
    MyClass<int> mycls;
    mycls.func();
    /*
     *	ld: Undefined symbols:
     *	MyClass<int>::func(), referenced from:
     *		_main in main-561ab4.o
     *	clang++: error: linker command failed with exit code 1 (use -v to see invocation)
     */

    return 0;
}