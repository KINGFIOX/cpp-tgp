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

union B {
};

int main(void)
{
    std::cout << IsClass<A>::value << std::endl; // 1
    std::cout << IsClass<int>::value << std::endl; // 0
    std::cout << IsClass<B>::value << std::endl; // 0

    int A::*mp = nullptr; // 指向类 A 中的某一个成员变量，并且成员变量的类型是 int

    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<decltype(mp)>().pretty_name() << std::endl; // int A::*
}
