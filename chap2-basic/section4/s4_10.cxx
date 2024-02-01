#include <iostream>
#include <tuple>

using namespace std;

/**
 * @brief 可变参数模板
 *
 * @tparam T
 * @param t
 */
template <typename... T>
void myfuncTuple(const tuple<T...>& t)
{
}

int main(void)
{
    tuple<double, int, int> mt(12.5, 100, 52);
    cout << get<0>(mt) << endl;
    cout << get<1>(mt) << endl;
    cout << get<2>(mt) << endl;
    myfuncTuple(mt);
}