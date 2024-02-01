#include <iostream>

using namespace std;

/**
 * @brief
 *
 * @tparam T 一包类型
 * @param args 一包形参
 * 参数数量 与 类型数量必定是相同的
 */
template <typename... T>
void myvtfunct(T... args)
{
    cout << "---------- begin ----------" << endl;
    cout << sizeof...(args) << endl;
    cout << sizeof...(T) << endl;
    cout << "---------- end ----------" << endl;
}

int main(void)
{
    myvtfunct();

    /* 完全不指定类型 */
    myvtfunct(10, 20, 30, 40, 'a', 'b', "abc", myvtfunct<int>);

    /* 指定部分类型 */
    myvtfunct<double, double>(10, 20, 30, 40, 'a', 'b', "abc", myvtfunct<int>);
}