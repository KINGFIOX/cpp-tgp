/**
 * @file s4_7.cxx 非类型模板参数
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstdio>
#include <iostream>

using namespace std;

template <int... FTArgs>
class myClassT {
    myClassT()
    {
        printf("myClassT::myClassT()泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 偏特化
 *
 * @tparam First
 * @tparam Others
 */
template <int First, int... Others>
class myClassT<First, Others...> : private myClassT<Others...> {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu, First=%d\n", this, sizeof...(Others), First);
    }
};

int main(void)
{
}