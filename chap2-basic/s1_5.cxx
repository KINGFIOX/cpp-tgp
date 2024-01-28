/**
 * @file s1_5.cxx 缺省模板参数
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

using namespace std;

int mf1(int tmp1, int tmp2)
{
    return 1;
}

int mf2(int tmp1, int tmp2)
{
    return 2;
}

typedef int (*FuncType)(int, int);

template <typename T, typename F = FuncType>
void testfunc(T i, T j, F funcpoint = mf1)
{
    cout << funcpoint(i, j) << endl;
}

int main(void)
{
    testfunc(15, 16);
    testfunc(15, 16, mf2);
}