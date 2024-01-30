#include <iostream>

using namespace std;

// 声明 1
template <typename T, typename U, typename V = int, typename W = char>
struct TC;

// 声明 2
template <typename T, typename U = double, typename V, typename W>
struct TC;

// 定义
template <typename T, typename U, typename V, typename W>
struct TC { };

typedef TC<int> TF_TC;
using U_TC = TC<double>; // 这种编程语法比较好

int main(void)
{
    TF_TC myTC1;
    U_TC myTC2;
}