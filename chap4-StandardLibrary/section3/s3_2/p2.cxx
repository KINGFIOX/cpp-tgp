#include <iostream>
#include <type_traits>
#include <utility>

using namespace std;

class ACPABL { };

class BCPABL {
public:
    BCPABL& operator=(const BCPABL& tmpObj)
    {
        return *this;
    }
};

class CCPABL {
public:
    /* 标记为 delete 或者是 私有，那么就不能被拷贝 */
    BCPABL& operator=(const CCPABL& tmpObj) = delete;
};

/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct IsCopyAssignable : std::false_type { };

/* 特化版本 */
template <typename T>
// struct IsCopyAssignable<T, std::void_t</* 关键是这里面怎么填写 */>>
struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&>())>>
    : std::true_type { };
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T>() = std::declval<const T&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&&>() = std::declval<const T&>()
/* std::declval<T&>() 假想创建了一个 T 的左值引用对象 */

/**
 * struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T>() = std::declval<const T&>()
 * 如果是 T 而不是 T& ，那么下面的输出结果是 0 1 1 0
 */

/**
 * struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&&>() = std::declval<const T&>()
 * 如果是 T&& 而不是 T& ，那么下面的输出结果是 0 1 1 0
 */

// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&&>()
// 上面这三种，似乎没什么影响，都是 1 1 1 0

// 类对象本身都是左值，那么 std::declval<T&>() = std::declval<const T&>() 是允许的
/* std::declcal<T>() -> T&& ，那么就会 */
/* std::declcal<T&>() -> T& && （引用折叠） <==> T& */
/* std::declcal<T&&>() -> T&& && （引用折叠） <==> T&& ，右值引用 */

int main(void)
{
    BCPABL bObj1;
    BCPABL bObj2;

    cout << IsCopyAssignable<int>::value << endl; // 1
    cout << IsCopyAssignable<ACPABL>::value << endl; // 1
    cout << IsCopyAssignable<BCPABL>::value << endl; // 1
    cout << IsCopyAssignable<CCPABL>::value << endl; // 0
}