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
class myClassT<First, Others...> // : private myClassT<Others...> // 偏特化
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
        , m_o(paro...)
    // , myClassT<Others...>(paro...) /* 传递给父类的构造函数 */
    {
        cout << "---------- begin ----------" << endl;
        printf("myclassT::myclassT(parf, ...paro)执行了, this=%p\n", this);
        cout << m_i << endl;
        cout << "---------- end ----------" << endl;
    }

    First m_i;
    myClassT<Others...> m_o;
};

int main(void)
{
    myClassT<int, int, double, float>(10, 20, 2.1, 2.3f);
    /*
            myClassT::myClassT() 泛化版本执行了, this=0x16fd96d1c
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d18
                        2.3
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d10
                        2.1
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d08
                        20
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d00
                        10
                        ---------- end ----------
    */
}