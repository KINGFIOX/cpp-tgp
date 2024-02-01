#include <cstdio>
#include <iostream>

using namespace std;

// 主模板定义（泛化版本的类模板）
template <typename... Args>
class myClassT {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 泛化版本执行了, this=%p\n", this);
    }
};

// // 也有的人不写定义，只写声明（主模板）
// template <typename... Args>
// class myClassT; /* 这个只写声明能正确，是因为没有用 泛化版本 创建对象。但是可以使用偏特化、特化创建对象 */

template <typename First, typename... Others>
class myClassT<First, Others...> : private myClassT<Others...> // 偏特化
{
public:
    myClassT()
        : m_i(0)
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%d\n", this, sizeof...(Others));
    }

    First m_i;
};

int main(void)
{
    myClassT<int, float, double> myc;
    /**
     * myClassT::myClassT() 泛化版本执行了, this=0x16f53eaf0
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=0
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=1
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=2
     */
}