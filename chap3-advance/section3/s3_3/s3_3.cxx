#include <iostream>

using namespace std;

int getData()
{
    return 3;
}

void funcLast3(int&& v1)
// void funcLast3(int v1) /* 当然也是可以哒 */
{
    cout << "v1=" << v1 << endl;
}

void funcMiddle_Temp()
{
    auto&& result = getData(); /* 万能引用，返回的是将亡值 */
    // getData() = 10; /* 错误：expression is not assignable，是不是左值，看看能不能放到等号左边 */

    result += 10; /* 模拟了对 result 的各种计算 */

    funcLast3(
        /* decltype 可以推断出类型 */
        std::forward<decltype(result)>(result));
}

int main(void)
{
}