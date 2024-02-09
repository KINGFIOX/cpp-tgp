#include <iostream>
#include <type_traits>

template <typename T, typename U>
struct IsSameType : std::false_type { };

template <typename T>
struct IsSameType<T, T> : std::true_type { };

/* ---------- ---------- */

template <typename T>
class IsDefaultConstructible {
private:
    /* 函数模板，只有声明，没有实现 */
    template <typename = decltype(T())>
    /* 错误： No matching constructor for initialization of 'B' */
    static std::true_type test(void*);

    /* 重载版本，这个 test 接受 可变形参，这个 typename = int 的 int 是随便给的 */
    /* 编译器会 优先选择：有具体形参的版本，带 ... 形参的 test 具有最低被匹配的优先级 */
    template <typename = int>
    static std::false_type test(...);

public:
    static constexpr bool value = IsSameType<decltype(test(nullptr)), std::true_type>::value;
};

// 如果 T 支持默认构造。那么 value = IsSameType<decltype(test(nullptr), std::true_type)>::value
// decltype(test(nullptr)) 推断 test(nullptr) 的返回类型
// test(nullptr) 优先选择 template<typename = decltype(T())> static std::true_type test(void*);
// 如果 T() 能够被默认构造出来，就没啥问题，那么 test(nullptr) 就是返回 std::true_type，
// 否则就是返回 std::false_type

// 核心特点：使用了同名，但是不同返回类型的：成员函数模板

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

class B {
public:
    B(int tmpVal) { }
};

int main(void)
{
    std::cout << IsDefaultConstructible<int>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<A>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<B>::value << std::endl; // 0
}