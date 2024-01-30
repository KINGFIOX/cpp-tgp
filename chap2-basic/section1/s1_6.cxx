#include <iostream>

using namespace std;

/**
 * @brief
 *
 * @tparam T
 * @tparam U
 * @tparam value 模板参数，长得像是 函数的形参
 * @param tv1
 * @param tv2
 * @return auto
 */
template <typename T, typename U, int value = 100>
auto Add(T tv1, U tv2)
{
    cout << "val = " << value << endl;
    return tv1 + tv2;
}

int main(void)
{
    cout << Add<float, float, 22>(22.3f, 11.8f) << endl;
}