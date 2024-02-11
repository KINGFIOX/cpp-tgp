/**
 * @file p3.cxx
 * @author your name (you@domain.com)
 * @brief 老式的 typelist 设计
 * @version 0.1
 * @date 2024-02-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstddef>
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

/**
 * @brief 空表
 *
 */
class NullTypeList { };

template <typename T, typename U = NullTypeList>
class TypeList {
public:
    using Head = T;
    using Tail = U;
    // 用意：将 typelist 分为两个部分：头 + 其余列表
    // typelist<int, nulltypelist> 有一个元素
    // typelist<int, typelist<double, nulltypelist>>，这看起来有点笨，可变参是：typelist<int, double>
    // 为了简化书写，可以定义一些宏，看上面
};

#define MAC_TYPELIST1(T1) TypeList<T1, NullTypeList>
#define MAC_TYPELIST2(T1, T2) TypeList<T1, MAC_TYPELIST1(T2)>
#define MAC_TYPELIST3(T1, T2, T3) TypeList<T1, MAC_TYPELIST2(T2, T3)>

using TPL_NM3 = MAC_TYPELIST3(int, double, char);
using TPL_NM4 = NullTypeList;

/* ---------- ---------- front ---------- ---------- */

template <typename TL>
class front {
public:
    using type = typename TL::Head;
};

/* ---------- ---------- size ---------- ---------- */

/**
 * @brief 泛化版本只声明，不实现
 *
 * @tparam TL
 */
template <typename TL>
class size;

/**
 * @brief 特化版本
 *
 */
template <>
class size<NullTypeList> {
public:
    static inline size_t value = 0;
};

template <typename T, typename U>
class size<TypeList<T, U>> {
public:
    static inline size_t value = 1 + size<U>::value;
};

/* ---------- ---------- pop_front ---------- ---------- */

template <typename TL>
class pop_front;

/**
 * @brief 特化版本
 *
 */
template <>
class pop_front<NullTypeList>;

/**
 * @brief 特化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
class pop_front<TypeList<T, U>> {
public:
    using type = U;
};

/* ---------- ---------- push_front ---------- ---------- */

template <typename TL, typename NewElem>
class push_front {
public:
    using type = TypeList<NewElem, TL>;
};

/* ---------- ---------- push_back ---------- ---------- */

template <typename TL, typename NewElem>
class push_back;

/**
 * @brief base case
 *
 * @tparam NewElem
 */
template <typename NewElem>
class push_back<NullTypeList, NewElem> {
public:
    using type = TypeList<NewElem, NullTypeList>;
};

template <typename Head, typename Tail, typename NewElem>
class push_back<TypeList<Head, Tail>, NewElem> {
private:
    using rec = push_back<Tail, NewElem>::type; // 这个相当是 TL<>
public:
    using type = TypeList<Head, rec>;
};

/* ---------- ---------- is_empty ---------- ---------- */

template <typename TL>
class is_empty : std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam
 */
template <>
class is_empty<NullTypeList> : std::true_type { };

/* ---------- ---------- find ---------- ---------- */

#if 0

/**
 * @brief 泛化版本
 *
 * @tparam TL
 * @tparam index_v
 */
template <typename TL, std::size_t index_v>
class find : public find<typename pop_front<TL>::type, index_v - 1> { };

/**
 * @brief 特化版本
 *
 */
template <typename TL>
class find<TL, 0> : public front<TL> { }; // 递归的出口

#else

/**
 * @brief 泛化版本
 *
 * @tparam TL
 * @tparam index_v
 */
template <class TL, std::size_t index_v>
class find;

/**
 * @brief 特化版本 base case
 *
 * @tparam Head
 * @tparam Tail
 */
template <class Head, class Tail>
class find<TypeList<Head, Tail>, 0> {
public:
    using type = Head;
};

/**
 * @brief 特化版本
 *
 * @tparam Head
 * @tparam Tail
 * @tparam index_v
 */
template <class Head, class Tail, std::size_t index_v>
class find<TypeList<Head, Tail>, index_v> {
public:
    using type = typename find<Tail, index_v - 1>::type;
};

#endif

/* ---------- ---------- find_nonstrict 越界不报错 ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 * @tparam index_v
 * @tparam DefaultType
 */
template <class TPLT, std::size_t index_v, typename DefaultType = NullTypeList>
class find_noStrict {
public:
    using type = DefaultType; // 越界返回的类型

    // 发现 find 与 find_noStrict 的差别主要是在：泛化版本中
};

/**
 * @brief 特化版本 base case
 *
 * @tparam Head
 * @tparam Tail
 * @tparam DefaultType = NullTypeList 有默认值，特化不用指定默认值
 */
template <class Head, class Tail, typename DefaultType>
class find_noStrict<TypeList<Head, Tail>, 0, DefaultType> {
public:
    using type = Head;
};

template <class Head, class Tail, std::size_t index_v, typename DefaultType>
class find_noStrict<TypeList<Head, Tail>, index_v, DefaultType> {
public:
    using type = typename find_noStrict<Tail, index_v - 1>::type;
};

#if 0

int main(void)
{
    std::cout << typeName<front<TPL_NM3>::type>() << std::endl; // int

	std::cout << size<TPL_NM3>::value << std::endl; // 3

	std::cout << typeName<pop_front<TPL_NM3>::type>() << std::endl;
    // TypeList<double, TypeList<char, NullTypeList>>

	std::cout << typeName<push_front<TPL_NM3, bool>::type>() << std::endl;
    // TypeList<bool, TypeList<int, TypeList<double, TypeList<char, NullTypeList>>>>

	std::cout << typeName<find<TPL_NM3, 2>::type>() << std::endl; // char
    std::cout << typeName<find_noStrict<TPL_NM3, 1>::type>() << std::endl; // double
    std::cout << typeName<find_noStrict<TPL_NM3, 3>::type>() << std::endl; // NullTypeList

}

#else

int main(void)
{
}

#endif