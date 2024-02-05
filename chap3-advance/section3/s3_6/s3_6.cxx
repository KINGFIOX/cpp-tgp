#include <cstddef>
#include <cstring>
#include <iostream>
#include <utility>

using namespace std;

template <typename F, typename... T>
void funcMiddle_Temp1(F f, T&&... t)
{
    f(std::forward<T>(t)...);
}

template <typename F, typename... T>
/* 这里的 decltype 不会浪费资源，decltype 与 sizeof 都是不求值的 */
auto funcMiddle_Temp2(F f, T&&... t) -> decltype(f(std::forward<T>(t)...))
{
    return f(std::forward<T>(t)...);
}

int funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
    return v1 + v2;
}

void funcLast4(char* p)
{
    if (p != NULL) {
        strncpy(p, "abc", 3);
    }
}

int main(void)
{
    int j = 70;
    int k = funcMiddle_Temp2(funcLast, 20, j); // 91
    cout << "j=" << j << endl; // 71
    cout << "k=" << k << endl; // 91

    // funcMiddle_Temp1(funcLast4, NULL);
    // /* 错误：f(std::forward<T>(t)...); ：Cannot initialize a parameter of type 'char *' with an rvalue of type 'long' */
    // /* 解决方案：使用nullptr */

    funcMiddle_Temp1(funcLast4, nullptr);
}