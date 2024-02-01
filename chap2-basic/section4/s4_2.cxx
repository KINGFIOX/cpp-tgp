#include <iostream>

using namespace std;

#define ce 1

#if ce

/* 同名的递归终止函数（这是一个普通函数，而不是函数模板），必须放在 “展开函数” 的 上面 */
void myvtfunct()
{
    cout << "参数包展开时执行了递归终止函数 myvtfunct()" << endl;
}

/**
 * @brief 形参包展开函数
 *
 * @tparam T
 * @tparam U
 * @param firstArg
 * @param otherArgs
 */
template <typename T, typename... U>
void myvtfunct(T firstArg, U... otherArgs)
{
    cout << "收到的参数值: " << firstArg << endl;
    myvtfunct(otherArgs...); /* 递归调用，注意：塞进来的是一包形参，这里的...不能省略 */
}

#else

template <typename T, typename... U>
void myvtfunct(T firstArg, U... otherArgs)
{
    if constexpr (sizeof...(U) > 0) {
        cout << "收到的参数值: " << firstArg << endl;
        myvtfunct(otherArgs...); /* 递归调用，注意：塞进来的是一包形参，这里的...不能省略 */
    } else {
        /* ... */
    }
}

#endif

int main(void)
{
    myvtfunct(10, 20, 12.7, 'c', "fuck", myvtfunct<int>);
}
