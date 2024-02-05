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

// A&& aYinObj(); /* 声明，但是没有实现 */
// /**
//  * 该函数返回类型是 A&&，可以看成是一返回了一个 A&& 类型的对象，这种对象就可以看成是 类 A 对象。
//  * 这种对象去调用 myfunc 的写法就没有错误 ---> aYinObj.myfunc(); 语法上也没啥毛病
//  * decltype(aYinObj().myfunc()) myDBLvar; // 定义了一个 double 类型的 左值变量（编译链接都不报错）
//  * 		decltype 推断的时候，不需要调用函数（不求值表达式）
//  */

int main(void)
{
    using YT = decltype(std::declval<A>()); /* 这个圆括号不能缺了，缺了，不报错，但是含义完全变了 */

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<YT>().pretty_name() << endl; // T=A&&

    cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(
        // 注意一下这种写法
        declval<A>().myfunc() // 注意一下这种写法
        )>()
                                                 .pretty_name()
         << endl; // myaobj.myfunc() 的返回类型=double
}