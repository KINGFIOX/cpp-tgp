#include <iostream>
#include <type_traits>
#include <utility>

template <typename FROM, typename TO>
struct IsConvertibleHelper {
private:
    static void testfunc(TO)
    {
    }

    /* 这里发生了 隐式类型转换；实参是 from，形参是 */
    template <typename = decltype(testfunc(std::declval<FROM>()))>
    /* 错误： No viable conversion from 'A' to 'B' */
    static std::true_type test(void*);

    static std::false_type test(...);

public:
    using type = decltype(test(nullptr));
};

template <typename FROM, typename TO>
struct IsConvertible : public IsConvertibleHelper<FROM, TO>::type { };

/* ---------- ---------- 测试代码 ---------- ----------  */

class A { };

class B : public A { };

int main(void)
{
    std::cout << IsConvertible<A, B>::value << std::endl;
    std::cout << IsConvertible<B, A>::value << std::endl;
}