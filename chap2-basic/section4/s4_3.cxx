#include <iostream>

using namespace std;

template <typename... T>
void myfunc(T... arg)
{
    cout << "myfunc(T... arg) 执行了!" << endl;
}

template <typename... T>
void myfunc(T*... arg)
{
    cout << "myfunc(T*... arg) 执行了!" << endl;
}

void myfunc(int arg)
{
    cout << "myfunc(int arg) 执行了!" << endl;
}

int main(void)
{
    myfunc(NULL); // myfunc(T... arg) 执行了!
    myfunc((void*)0); // myfunc(T*... arg) 执行了!
    myfunc(nullptr); // myfunc(T... arg) 执行了!
    myfunc((int*)nullptr); // myfunc(T*... arg) 执行了!
    cout << typeid(NULL).name() << endl; // l
    cout << sizeof(NULL) << endl; // 8
}
