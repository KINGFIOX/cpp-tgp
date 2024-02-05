#include <boost/type_index.hpp>
#include <cstdio>
#include <iostream>
#include <utility>

using namespace std;

class A {
public:
    A(int i) /* 构造函数 */
    {
        printf("A::A() 函数执行了, this=%p\n", this);
    }
    double myfunc()
    {
        printf("A::myfunc() 函数执行了, this=%p\n", this);
        return 12.1;
    }
};

int main(void)
{
    using YT = decltype(std::declval<A>()); /* 这个圆括号不能缺了，缺了，不报错，但是含义完全变了 */

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<YT>().pretty_name() << endl; // T=A&&

    A myaobj(10); // 创建对象
    cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(myaobj.myfunc())>().pretty_name() << endl; // myaobj.myfunc() 的返回类型=double
    /**
     * 观察现象，得到两条结论：
     * 1. 要得到 myfunc() 的返回类型，有些麻烦：要创建对象，创建对象的话还要传参，并且还要调用对象，尽管 decltype 不求值
     * 2. 当然，如果是私有函数，那么 decltype(myaobj.myfunc()) 就不允许了
     */
}