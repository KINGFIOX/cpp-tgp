#include <iostream>

using namespace std;

template <typename T>
T g_myVar {}; /* 初始化为0 */

template <>
char g_myVar<double> {};

template <typename T>
T g_myVar<T*> { 120 }; /* 要求 T* 必须依赖于 T */

int main(void)
{
    g_myVar<float> = 15.6f;
    g_myVar<int> = 13;

    cout << g_myVar<float> << endl; // 15.6
    cout << g_myVar<int> << endl; // 13
    cout << typeid(g_myVar<double>).name() << endl; // c

    cout << typeid(g_myVar<int*>).name() << endl;
    cout << g_myVar<int*> << endl;
}
