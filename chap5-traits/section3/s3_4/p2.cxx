#include <iostream>
#include <type_traits>
#include <utility>

/* ---------- ---------- is_same_type ---------- ---------- */

template <typename T, typename U>
struct IsSameType : std::false_type { };

template <typename T>
struct IsSameType<T, T> : std::true_type { };

template <typename Base, typename Derived>
class IsBaseOf {
private:
    template <typename T>
    static std::true_type test(T*);

    template <typename>
    static std::false_type test(void*);

    template <typename B, typename D>
    static auto test_middle() -> decltype(test<B>(static_cast<D*>(nullptr)));
    // 返回类型后置语法
    //

public:
    static constexpr bool value = IsSameType<
        std::integral_constant<bool,
            std::is_class<Base>::value && //
                std::is_class<Derived>::value&& //
                decltype(test_middle<Base, Derived>())::value>,
        /* 但是如果这里出现 C 私有继承 A，这是有问题的 */
        std::integral_constant<bool, true>>::value;
};

class A { };

class B : public A {
};

class C : private A {
};

int main(void)
{
    std::cout << IsBaseOf<A, B>::value << std::endl; // 1
    // std::cout << IsBaseOf<A, C>::value << std::endl; // 1
}
