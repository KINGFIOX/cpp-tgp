#include <iostream>

using namespace std;

template <typename T = int>
T g_myVar {}; /* 初始化为0 */

int main(void)
{
    g_myVar<> = 26;
    cout << g_myVar<> << endl;
    // cout << g_myVar << endl; /* 错误： Use of variable template 'g_myVar' requires template arguments */
}
