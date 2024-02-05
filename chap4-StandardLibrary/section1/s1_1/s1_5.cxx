/**
 * @file s1_4.cxx 自己模拟 declval ， 右值引用
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <boost/type_index.hpp>
#include <cstdio>
#include <iostream>

using namespace std;

class A {
public:
    A()
    {
        printf("A::A() 函数执行了, this=%p\n", this);
    }
    A(int i) /* 构造函数 */
    {
        printf("A::A(int) 函数执行了, this=%p\n", this);
    }
    double myfunc()
    {
        printf("A::myfunc() 函数执行了, this=%p\n", this);
        return 12.1;
    }

private:
    ~A() { }
};

template <typename T>
T&& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

int main(void)
{
    using boost::typeindex::type_id_with_cvr;

    cout << "mydeclval<A> => " << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name() << endl;
    cout << "mydeclval<A&> => " << type_id_with_cvr<decltype(mydeclval<A&>())>().pretty_name() << endl;
    cout << "mydeclval<A&&> => " << type_id_with_cvr<decltype(mydeclval<A&&>())>().pretty_name() << endl;
    /**
     * 下面三个相同，引用折叠
     * mydeclval<A> => A&&
     * mydeclval<A&> => A&
     * mydeclval<A&&> => A&&
     */
}