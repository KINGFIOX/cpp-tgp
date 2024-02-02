/**
 * @file s6_6.cxx
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

using namespace std;

template <typename... T>
class Base : public T... /* 这种 继承 T... 被称为混入 */ {
public:
    /**
     * 主要注意的问题是：如果 T... 中的某个类，包含 virtual myfunc，
     * 		那么显然，这里的 Base::myfunc() 就自动变成 virtual，
     *		并且，如果父类指针指向子类对象，那么这个问题就有趣了，
     *		可能会导致
     *
     */
    void myfunc()
    {
        cout << "Base::myfunc() 执行了!" << endl;
    }
};

template <typename... T>
class Derived : public Base<T...> {
public:
    void myfunc()
    {
        cout << "Derived::myfunc() 执行了!" << endl;
    }
};

class A {
};

class VirA {
    virtual void myfunc()
    {
    }
};

int main(void)
{
    Base<A>* pb1 = new Derived<A>; /* 父类指针指向子类对象 */
    pb1->myfunc(); // Base::myfunc() 执行了!

    /* 这里通过指定不同的参数，看是不是要虚拟化 */

    Base<VirA>* pb2 = new Derived<VirA>; /* 父类指针指向子类对象 */
    pb2->myfunc(); // Derived::myfunc() 执行了!
}
