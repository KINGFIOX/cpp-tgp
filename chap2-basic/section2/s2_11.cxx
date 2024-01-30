#include <iostream>

using namespace std;

/**
 * @brief 声明 1
 *
 * @tparam T
 * @tparam U
 * @tparam V
 * @tparam W
 */
template <typename T, typename U, typename V = int, typename W = char>
struct TC;

// /* 错误： V W : Template parameter redefines default argument */
// template <typename T, typename U = double, typename V = int, typename W = char>
// struct TC;

// 声明 2
template <typename T, typename U = double, typename V, typename W>
struct TC;

// 定义
template <typename T, typename U, typename V, typename W>
struct TC { };

int main(void)
{
    TC<char> mytc; /* <char, default, default, default> */
}