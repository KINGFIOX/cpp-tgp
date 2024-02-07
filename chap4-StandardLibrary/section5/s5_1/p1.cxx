#include <functional>
#include <iostream>

using namespace std;

void myfunc(int tmpValue)
{
    cout << "myfunc 执行了: tmpValue = " << tmpValue << endl;
}

void callObjFunc(std::function<void(int)> cObj)
{
    cObj(120);
}

int main(void)
{
    callObjFunc(myfunc);
    callObjFunc([](int tmpValue) -> void {
        cout << "lambda 表达式 执行了: tmpValue = " << tmpValue << endl;
    });
    return 0;
}