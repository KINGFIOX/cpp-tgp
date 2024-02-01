#include <complex>
#include <cstdio>
#include <iostream>

using namespace std;

template <typename... T>
auto unary_left(T... args)
{
    return (... + args); /* 折叠表达式，圆括号不能省略 */
}

template <typename... T>
auto unary_right(T... args)
{
    return (args + ...);
}

template <typename... T>
auto binary_left(T... args)
{
    (cout << ... << args);
}

template <typename... T>
auto binary_right(T... args)
{
    (cout << ... << args);
}

template <typename... T>
auto print_result(T const&... args)
{
    (cout << ... << args) << "结束" << endl;
    return (... + args);
}

template <typename... T>
void print_calc(T const&... args)
{
    cout << print_result(2 * args...) << endl;
    /**
     * (2 * args) 不行
     * (args... * 2) 不行
     * (args * 2...) 不行
     * (args * 2 ...) 可以
     * ((args * 2) ...) 可以
     */
}

int main(void)
{
    // (((1 + 2) + ...) + n )
    cout << unary_left(10, 20, 30, 30.1) << endl;
    // ( 1 + (... + (n-1 + n)))
    cout << unary_right(2, 4, 6, 8) << endl;

    // binary_left("10\n", "abc\n", "def\n");
    // /**
    //  *  10
    //  *  20
    //  *  abc
    //  *  def
    //  */
    // print_result(10, 20, 30, 40);
    print_calc(10, 20, 30, 40);
}