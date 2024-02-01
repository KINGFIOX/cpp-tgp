#include <cstdio>
#include <iostream>

using namespace std;

template <typename... Args>
class myClassT {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 取材于 C++ 标准库的 tuple
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class myClassT<First, Others...> : private myClassT<Others...> // 偏特化
{
public:
    myClassT()
        : m_i(0)
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu\n", this, sizeof...(Others));
    }

    /**
     * @brief Construct a new my Class T object 有参构造
     *
     * @param parf
     * @param paro
     */
    myClassT(First parf, Others... paro)
        : m_i(parf)
        , myClassT<Others...>(paro...) /* 传递给父类的构造函数 */
    {
        cout << "---------- begin ----------" << endl;
        printf("myclassT::myclassT(parf, ...paro)执行了, this=%p\n", this);
        cout << m_i << endl;
        cout << "---------- end ----------" << endl;
    }

    First m_i;
};

/**
 * @brief 即使有了这个特化版本，上面的 泛化版本依然是不能删除的
 *
 * @tparam
 */
template <>
class myClassT<> {
public:
    myClassT()
    {
        printf("特殊的特化版本执行了: %p\n", this);
    }
};

int main(void)
{
    myClassT<int, float, double> myc;
    /**
     * 特殊的特化版本执行了: 0x16b0c6d10
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=0
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=1
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=2
     */
}