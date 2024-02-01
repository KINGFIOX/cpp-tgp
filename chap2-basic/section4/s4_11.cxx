/**
 * @file s4_11.cxx 利用 tuple 展开参数包
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <tuple>

using namespace std;

/**
 * @brief 泛化版本
 *
 * @tparam cnt 用于统计
 * @tparam maxCNT 参数的数量
 * @tparam T
 */
template <int cnt, int maxCNT, typename... T>
class myClassT {
public:
    static void sfunc(const tuple<T...>& t)
    {
        cout << "value = " << get<cnt>(t) << endl; /* 可以把每个参数都取出来并输出 */
        myClassT<cnt + 1, maxCNT, T...>::sfunc(t); /* 递归调用 */
    }
};

/**
 * @brief 偏特化版本，用于结束递归调用
 *
 * @tparam maxCNT
 * @tparam T
 */
template <int maxCNT, typename... T>
class myClassT<maxCNT, maxCNT, T...> /* 通过这个尖括号，反推上面参数 */ {
public:
    static void sfunc(const tuple<T...>& t)
    {
        /* 这里保持为空就行了，实际上，这个 maxCNT 正好是到了这个越界位置了 */
    }
};

/**
 * @brief 包装一下
 *
 * @tparam T
 * @param t
 */
template <typename... T>
void myfuncTuple(const tuple<T...>& t)
{
    myClassT<0, sizeof...(T), T...>::sfunc(t);
}

int main(void)
{
    tuple<double, int, int> mt(12.5, 100, 52);
    myfuncTuple(mt);
}