/**
 * @file s1_3.cxx 自己模拟 declval ， 但是是原始类型
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
T mydeclval() noexcept; /* 我们自己模拟一个 declval，但是是原始类型 */

int main(void)
{
    using boost::typeindex::type_id_with_cvr;

    cout << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name() << endl;
    // 这里并不是要调用 myfunc，因此语义上没有要创建一个 临时对象

    // cout << type_id_with_cvr<decltype(mydeclval<A>().myfunc())>().pretty_name() << endl;
    // 虽然不是创建一个 真实的对象，mydeclval<A>() 看起来是要作为一个操作数，参与计算。因此要遵循一个 语义限制
    // 这里的语义是：创建一个临时对象
    // 返回原始类型，就是有一种 创建临时对象的语义（将亡值）
    // 与 decltype 类似的，还有 sizeof
    /**
     * 错误：
     * error: s1_3.cxx:47:77: error: temporary of type 'A' has private destructor
     * 47 |     cout  << type_id_with_cvr<decltype(mydeclval<A>().myfunc())>().pretty_name() << endl;
     * |                                                                        ^
     * s1_3.cxx:35:5: note: declared private here
     * 35 |     ~A() { }
     * |     ^
     * 1 error generated.
     * > in s1_3.cxx
     *
     */
}