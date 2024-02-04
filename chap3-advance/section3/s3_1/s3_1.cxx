#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

// void funcLast(int v1, int& v2)
// {
//     ++v2;
//     cout << v1 + v2 << endl;
// }

// /**
//  * @brief
//  *
//  * @tparam F 函数指针类型
//  * @tparam T1
//  * @tparam T2
//  * @param f
//  * @param t1
//  * @param t2
//  */
// template <typename F, typename T1, typename T2>
// void funcMiddle_Temp(F f, T1 t1, T2 t2)
// {
//     f(t1, t2);
// }

void funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

/**
 * @brief 万能引用失败的情况 ---> 右值引用
 *
 * @param v1
 * @param v2
 */
void funcLast2(int&& v1, int& v2)
{
    cout << v1 << endl;
    cout << v2 << endl;
}

/**
 * @brief
 *
 * @tparam F 函数指针类型
 * @tparam T1
 * @tparam T2
 * @param f
 * @param t1
 * @param t2
 */
// template <typename F, typename T1, typename T2>
// void funcMiddle_Temp(F f, T1 t1, T2 t2)
// {
//     f(t1, t2);
// }

template <typename F, typename T1, typename T2>
void funcMiddle_Temp(F f, T1&& t1, T2&& t2)
{
    f(t1, t2);
}

template <typename F, typename T1, typename T2>
void funcMiddle_Temp2(F f, T1&& t1, T2&& t2)
{
    f(
        std::forward<T1>(t1), // T1=int
        std::forward<T2>(t2) // T2=int&
    );
}

// #ifdef INSIGHTS_USE_TEMPLATE
// template <>
// void funcMiddle_Temp<void (*)(int, int&), int, int&>(void (*f)(int, int&),
//     int&& t1, int& t2)
// {
//     f(t1, t2);
// }
// #endif

int main(void)
{
    // int i = 50;
    // funcLast(41, i); // 92

    // int j = 70;
    // funcMiddle_Temp(funcLast, 20, j); // 91
    // cout << j << endl; // 70

    int j = 70;
    funcLast2(20, j); // 20 70

    // funcMiddle_Temp(funcLast2, 20, j);
    // /**
    //  * f(t1, t2) : 错误: rvalue reference to type 'int' cannot bind to lvalue of type 'int'
    //  *   ^~
    //  * 首先 t1 是右值引用类型，但是 t1 本身是左值，不能将左值传递给右值引用
    //  */

    funcMiddle_Temp2(funcLast, 20, j);
}