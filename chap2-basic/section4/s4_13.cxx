#include <cstdio>

using namespace std;

/**
 * @brief 泛化版本（不用加尖括号）
 *
 * @tparam Args
 */
template <typename... Args>
class myCLST {
public:
    myCLST()
    {
        printf("myClassT::myClassT, this=%p, sizeof...(Args)=%lu\n", this, sizeof...(Args));
    }
};

/**
 * @brief 偏特化（需要加尖括号）
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class myCLST<First, Others...> {
public:
    myCLST()
    {
        printf("myClassT::myClassT, this=%p, sizeof...(Ohters)=%d\n", this, sizeof...(Others));
    }
};

/**
 * @brief 带一个参数的偏特化版本
 *
 * @tparam Arg
 */
template <typename Arg>
class myCLST<Arg> {
public:
    myCLST()
    {
        printf("myClassT<Arg>::myClassT, this=%p\n", this);
    }
};

/**
 * @brief 带两个参数的偏特化版本
 *
 * @tparam Arg1
 * @tparam Arg2
 */
template <typename Arg1, typename Arg2>
class myCLST<Arg1, Arg2> {
public:
    myCLST()
    {
        printf("myClassT<Arg1, Arg2>::myClassT, this=%p\n", this);
    }
};

int main(void)
{
    myCLST<> myc0; /* myClassT::myClassT, this=0x16f0e6d0f, sizeof...(Args)=0 */
    myCLST<int> myc1; /* myClassT<Arg>::myClassT, this=0x16cfdad0f */
    myCLST<int, float> myc2; /* myClassT<Arg1, Arg2>::myClassT, this=0x16cfdad0e */
    myCLST<int, float, long> myc3; /* myClassT::myClassT, this=0x16cfdad0d, sizeof...(Ohters)=2 */
}
