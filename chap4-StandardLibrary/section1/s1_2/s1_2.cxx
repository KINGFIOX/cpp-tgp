#include <boost/type_index.hpp>
#include <iostream>
#include <utility>

using namespace std;

int myfunc(int a, int b)
{
    return a + b;
}

template <typename T_F, typename... U_Args>
decltype(std::declval<T_F>()(std::declval<U_Args>()...)) TestFnRtnImpl(T_F func, U_Args... args)
/* std::declval<> 中的 U_Args 写成 U_Args&、U_Args&& 都一样 */
/* std::declval<T_F>()(std::declval<U_Args>()...) <== 根据函数类型、函数参数类型 推导函数的返回值 */
/* std::declval<T_F>() ---> int(* &&)(int, int) ==> 可以简单理解成是 int(*)(int, int) */
/* (std::declval<U_Args>()...) 这种写法，推导出来的是 两个 int&& */
{
    auto rtnValue = func(args...);
    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(std::declval<T_F>())>().pretty_name() << endl; // int (*&&)(int, int)
    return rtnValue;
}

int main(void)
{
    auto result = TestFnRtnImpl(myfunc, 89, 64);
    /**
     * T_F 被推断成 int(*)(int, int)
     * U_Args : int, int
     */
    cout << result << endl;
    // decltype(int); // 错误：expected '(' for function-style cast or type construction
    // decltype 里面都是跟着：对象，并不是类，因此不能：
    // decltype(T_F(U_ARGS...))
}