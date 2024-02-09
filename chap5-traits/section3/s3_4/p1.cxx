#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

/* ---------- ---------- is_same_type ---------- ---------- */

template <typename T, typename U>
struct IsSameType : std::false_type { };

template <typename T>
struct IsSameType<T, T> : std::true_type { };

/* ---------- ---------- is_class ---------- ---------- */

template <typename T>
class IsClass {
private:
    /* 这里的 U 实际上就是 T，看我们调用的时候传入的参数。这里是为了：类型推断 */
    /* int U::* 表示：成员变量指针，省略掉了 指针名 */
    /* 只有：类类型，或者是 union 类型，才能匹配到这个 test(int U::*)，否则 SFINAE */
    template <typename U>
    static std::integral_constant<bool, !std::is_union<U>::value> test(int U::*);

    template <typename>
    static std::integral_constant<bool, false> test(...);

public:
    static constexpr bool value = IsSameType<
        decltype(test<T>(nullptr)),
        std::integral_constant<bool, true>>::value;
};

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

class B : public A {
};

class C : private A {
};

int main(void)
{
    std::cout << std::is_base_of<A, B>::value << std::endl; // 1
    std::cout << std::is_base_of<A, C>::value << std::endl; // 1
}
