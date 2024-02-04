#include <algorithm>
#include <iostream>

using namespace std;

void printInfo(int& t)
{
    cout << "printInfo() 参数类型是 左值引用" << endl;
}

void printInfo(int&& t)
{
    cout << "printInfo() 参数类型是 右值引用" << endl;
}

template <typename T>
void TestF(T&& t)
{
    printInfo(
        std::forward<T>(t));
}

int main(void)
{
    int i = 100;
    TestF(10);
    /**
     * printInfo() 参数类型是 右值引用
     */
    TestF(i);
    /**
     * printInfo() 参数类型是 左值引用
     */
    TestF(std::move(i));
    /**
     * printInfo() 参数类型是 右值引用
     */
    return 0;
}