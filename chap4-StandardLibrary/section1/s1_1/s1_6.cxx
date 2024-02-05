/**
 * @file s1_4.cxx
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

class ALR {
public:
    void onAnyValue()
    {
        cout << "ALR::onAnyValue() 函数执行了!" << endl;
    }
    void onLValue() & /* 限定符 &，只能被左值对象调用 */
    {
        cout << "ALR::onLValue() 函数执行了!" << endl;
    }
    // void onLvalue() && /* 限定符 &&，只能被右值对象调用 */ // 这种重载是 ✅ 的
    // {
    //     cout << "ALR::onAnyValue() 函数执行了!" << endl;
    // }

    void onRValue() &&
    {
        cout << "ALR::onRValue() 函数执行了!" << endl;
    }
};

#define R 1

#if R

template <typename T>
T&& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

#else

template <typename T>
T& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

#endif

int main(void)
{
    ALR alr; /* 左值类型 */
    alr.onAnyValue();

    /**
     * alr.onRValue();
     * 错误：'this' argument to member function 'onRValue' is an lvalue,
     *   but function has rvalue ref-qualifier
     */

    ALR().onRValue();

    /**
     * ALR().onLValue();
     * 'this' argument to member function 'onLValue' is an rvalue,
     *	  but function has non-const lvalue ref-qualifier
     *
     */

#if R

    decltype(mydeclval<ALR>().onAnyValue());
    decltype(mydeclval<ALR&>().onLValue()); // 此时 mydeclval -> ALR &
    // decltype(mydeclval<ALR&>().onRValue()); // 错误
    decltype(mydeclval<ALR&&>().onRValue()); // 此时 mydeclval -> ALR &&
    // decltype(mydeclval<ALR&&>().onLValue()); // 错误

#else

    /* mydeclval 返回左值引用，凡是 onRValue() 就是错误的 */

    decltype(mydeclval<ALR>().onAnyValue());
    decltype(mydeclval<ALR&>().onLValue()); // 此时 mydeclval -> ALR &
    // decltype(mydeclval<ALR&>().onRValue()); // 错误
    // decltype(mydeclval<ALR&&>().onRValue()); // 此时 mydeclval -> ALR &&
    decltype(mydeclval<ALR&&>().onLValue()); // ✅

#endif
}