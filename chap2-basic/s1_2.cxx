#include <iostream>

using namespace std;

/**
 * @brief 函数模板
 *
 * @tparam T
 * @param tmpValue
 */
template <typename T>
void myfunc(T tmpValue)
{
    cout << "myfunc(T)执行了!" << endl;
}

/* ---------- 重载版本 ---------- */

/**
 * @brief 重载的模板
 *
 * @tparam T
 * @param tmpValue
 */
template <typename T>
void myfunc(T* tmpValue)
{
    cout << "myfunc(T*)执行了!" << endl;
}

/**
 * @brief 非模板的重载
 *
 * @tparam T
 * @param tmpValue
 */
void myfunc(int tmpValue)
{
    cout << "myfunc(int)执行了!" << endl;
}

int main(void)
{
    myfunc(12); // myfunc(int)执行了!

    char* p = nullptr;
    myfunc(p); // myfunc(T*)执行了!

    myfunc(12.1); // myfunc(T)执行了!
}