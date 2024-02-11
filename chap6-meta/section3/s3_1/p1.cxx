#include <cxxabi.h>
#include <iostream>
#include <type_traits>

/* ---------- ---------- 打印名字 ---------- ---------- */

std::string demangle(const char* mangledName)
{
    int status = 0;
    std::unique_ptr<char, void (*)(void*)> result(
        abi::__cxa_demangle(mangledName, nullptr, nullptr, &status),
        std::free);
    return (status == 0) ? result.get() : mangledName;
}

template <typename T>
std::string typeName()
{
    return demangle(typeid(T).name());
}

/* ---------- ---------- typelist ---------- ---------- */

template <typename... T>
class typelist { };

/* ---------- ---------- front ---------- ---------- */

/**
 * @brief 泛化版本，用不到，所以值需要声明，存在的目的就是为了引出特化版本
 *
 * @tparam TPLT
 */
template <typename TPLT>
class front;

/**
 * @brief 特化版本。要学会如何填写 front< 这里 >
 * 		先填写 front < 这里 >，在根据 填写的内容，填写 template
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class front<typelist<First, Others...>> {
public:
    using type = First;
};

/* ---------- ---------- pop_front ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 */
template <typename TPLT>
class pop_front;

/**
 * @brief 特化版本
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class pop_front<typelist<First, Others...>> {
public:
    using type = typelist<Others...>;
};

/* ---------- ---------- push_front ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 * @tparam NewElem
 */
template <typename TPLT, typename NewElem>
class push_front;

template <typename... Elem, typename NewElem>
class push_front<typelist<Elem...>, NewElem> {
public:
    using type = typelist<NewElem, Elem...>;
};

/* ---------- ---------- push_back ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 * @tparam NewElem
 */
template <typename TPLT, typename NewElem>
class push_back;

template <typename... Elem, typename NewElem>
class push_back<typelist<Elem...>, NewElem> {
public:
    using type = typelist<Elem..., NewElem>;
};

/* ---------- ---------- replace_font ---------- ---------- */

template <typename TPLT, typename NewElem>
using replace_front = push_front<typename pop_front<TPLT>::type, NewElem>;
/* 然后调用的话就是： ::type */
/* 如果没有 pop_front 前面的 typename，那么就会报错 */
/* 错误：Template argument for template type parameter must be a type; did you forget 'typename'? */

/* ---------- ---------- size ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 */
template <typename TPLT>
class size;

template <typename... Args>
class size<typelist<Args...>> {
public:
    static inline size_t value = sizeof...(Args);
};

/* ---------- ---------- is_empty ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 */
template <typename TPLT>
class is_empty : public std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam
 */
template <>
class is_empty<typelist<>> : public std::true_type { };

/* ---------- ---------- 测试 ---------- ---------- */

using TPL_NM1 = typelist<int, double, char, bool>;
using TPL_NM2 = typelist<>;

int main(void)
{
    typelist<int, double, char, bool> s; // 这个容器可以用了，我们接下来就是看看怎么操作这些元素

    std::cout << typeName<front<TPL_NM1>::type>() << std::endl; // int
    // std::cout << typeName<front<TPL_NM2>::type>() << std::endl;
    // /* 错误：Implicit instantiation of undefined template 'front<typelist<>>' */
    // /* 就是 空的 typelist 就是不允许的（易于正确使用） */

    std::cout << size<TPL_NM1>::value << std::endl;

    std::cout << typeName<pop_front<TPL_NM1>::type>() << std::endl; // typelist<double, char, bool>

    std::cout << typeName<push_front<TPL_NM1, long>::type>() << std::endl; // typelist<long, int, double, char, bool>

    std::cout << typeName<push_back<TPL_NM1, long>::type>() << std::endl; // typelist<int, double, char, bool, long>

    std::cout << typeName<replace_front<TPL_NM1, long>::type>() << std::endl; // typelist<long, double, char, bool, long>

    std::cout << is_empty<TPL_NM2>::value << std::endl; // 1
}