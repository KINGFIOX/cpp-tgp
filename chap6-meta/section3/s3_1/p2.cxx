/**
 * @file p2.cxx
 * @author your name (you@domain.com)
 * @brief 完美版
 * @version 0.1
 * @date 2024-02-11
 *
 * @copyright Copyright (c) 2024
 *
 */

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

/* ---------- ---------- find ---------- ---------- */

/**
 * @brief 泛化版本，每次 pop_front，然后 索引号 --
 *		这里采用递归继承
 * @tparam TPLT
 */
template <typename TPLT, std::size_t index_v>
class find : public find<typename pop_front<TPLT>::type, index_v - 1> { };

/**
 * @brief 特化版本，我们一开始是写：特化版本的
 *
 * @tparam TPLT
 */
template <typename TPLT>
class find<TPLT, 0> : public front<TPLT> { };

/* ---------- ---------- get_maxsize_type ---------- ---------- */

/**
 * @brief 泛化版本
 *
 */
template <typename TPLT>
class get_maxsize_type {
private:
    using tl_first_elem = typename front<TPLT>::type; // 获取第一个元素
    using tl_remain = typename pop_front<TPLT>::type; // 去掉一个元素

    // 递归下去的 get_maxsize_type
    using tl_first_elem_rec = typename get_maxsize_type<tl_remain>::type;

public:
    using type = typename std::conditional<
        (sizeof(tl_first_elem) > sizeof(tl_first_elem_rec)),
        tl_first_elem,
        tl_first_elem_rec>::type;
};

/**
 * @brief 特化版本
 *
 */
template <>
class get_maxsize_type<typelist<>> {
public:
    using type = char[0]; // 长度为 0 的数组
};

/* ---------- ---------- 反转 reverse ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 * @tparam bool
 */
template <typename TPLT, bool = is_empty<TPLT>::value>
class reverse;

/**
 * @brief 非空的时候（特化版本）
 *
 * @tparam TPLT
 */
template <typename TPLT>
class reverse<TPLT, false> {
private:
    using _rid_front = typename pop_front<TPLT>::type;

    using tl_first_elem = typename front<TPLT>::type;
    using tl_result_rec = typename reverse<
        _rid_front, is_empty<_rid_front>::value>::type;
    // 把 front 插入尾部

public:
    using type = typename push_back<tl_result_rec, tl_first_elem>::type;
    // push 递归的 using。
};

/**
 * @brief 空的时候（特化版本）
 *
 * @tparam TPLT
 */
template <typename TPLT>
class reverse<TPLT, true> {
public:
    using type = typelist<>; /* 实际上也就是 TPLT */
};

/* ---------- ---------- 测试 ---------- ---------- */

using TPL_NM1 = typelist<int, double, char, bool>;
using TPL_NM2 = typelist<>;

#if 0

int main(void)
{
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

    std::cout << typeName<find<TPL_NM1, 3>::type>() << std::endl; // bool，正确的
    // std::cout << typeName<find<TPL_NM1, 4>::type>() << std::endl; // bool，正确的
    // 如果是错误的下标，编译的时候就会报错。

	std::cout << sizeof(char[0]) << std::endl;
    std::cout << sizeof(get_maxsize_type<typelist<>>::type) << std::endl;
    std::cout << typeName<get_maxsize_type<TPL_NM1>::type>() << std::endl; // double
    // using TPL_NM1 = typelist<int, double, char, bool>;
    // max_type<typelist<int, double, char, bool>>::type
    // -> first = int；rec = max_type<typelist<double, char, bool>>::type -> max
    // // max_type<typelist<double, char, bool>>::type
    // // -> first = double；rec = max_type<typelist<char, bool>>::type -> max
    // // // max_type<typelist<char, bool>>::type
    // // // -> first = char；rec = max_type<typelist<bool>>::type -> max
    // // // // max_type<typelist<bool>>::type
    // // // // -> first = bool；rec = max_type<typelist<>>::type -> max
    // // // // // max_type<typelist<>>::type = char[0]
    // // // // max(bool, char[0]) -> bool -> max_type<typelist<bool>>::type = bool
    // // // ... 向上传递

	std::cout << typeName<reverse<TPL_NM1>::type>() << std::endl; // double
    // using TPL_NM1 = typelist<int, double, char, bool>;
    // reverse<TL<int, double, char, bool>>
    // -> first = int, rec = reverse<double, char, bool> -> pb<rec, first>
    // // reverse<double, char, bool>
    // // -> first = double, rec = reverse<char, bool> -> pb<rec, first>
    // // // reverse<char, bool>
    // // // -> first = char, rec = reverse<bool> -> pb<rec, first>
    // // // // reverse<bool>
    // // // // -> first = bool, rec = TL<> -> pb<rec, int> = TL<bool>
    // // // reverse<bool> = TL<bool> -> pb< rev, char > -> TL<bool, char>
    // // reverse<bool, char> = TL<bool, char> -> pb< rev, double > -> TL<bool, char, double>
    // reverse<bool, char, double> = TL<bool, char, double>
    // -> pb< rev, int> -> TL<bool, char, double, int>
}

#else

int main(void)
{
}

#endif