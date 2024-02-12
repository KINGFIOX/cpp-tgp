/**
 * @file p1.cxx
 * @author your name (you@domain.com)
 * @brief 实现 make_integer_reverse（不懂）
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cxxabi.h>
#include <iostream>

/* ---------- ---------- integer_sequence ---------- ---------- */

/**
 * @brief
 *
 * @tparam T
 * @tparam Ints
 */
template <typename T, T... Ints>
struct IntegerSequence {
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

/* ---------- ---------- push_front ---------- ---------- */

template <typename IS, unsigned int NewElem> // INTSEQ代表整个的std::integer_sequence< ......>类型
struct IntSeq_PushFront; // 因为不使用泛化版本，所以泛化版本可以只声明不定义。

// 向std::integer_sequence末尾插入元素：特化版本
template <typename T, unsigned int NewElem, unsigned int... Elems> // 泛化版本一般要求可变参放在最后，特化版本一般无此要求
struct IntSeq_PushFront<IntegerSequence<T, Elems...>, NewElem> {
    /* ({a, b, c}, elem) ---> (elem, a, b, c) */
    using type = IntegerSequence<T, NewElem, Elems...>;
};

/* ---------- ---------- make_integer_sequence_reverse ---------- ---------- */

// 泛化版本
template <typename T, unsigned int N> /* 默认从 1 开始计数 */
struct MakeIntegerSequence_Reverse // 实现std::make_integer_sequence功能
{
    // 依次遍历出4,3,2,1，应该往末尾插入元素，所以还应该引入IntSeq_PushBack操作。
    using type = typename IntSeq_PushFront<typename MakeIntegerSequence_Reverse<T, N - 1>::type, N - 1>::type; // 这里理解成函数调用，把type理解成所调用的函数名
};

/**
 * @brief base case （特化版本）
 *
 * @tparam T
 */
template <typename T>
struct MakeIntegerSequence_Reverse<T, 1> // 这里是1，意味着递归到1就可以了
{
    using type = IntegerSequence<T, 0>; // 这是0，注意。
};

// 定义别名模板
template <typename T, unsigned int N>
using IntegerSequence_t = typename MakeIntegerSequence_Reverse<T, N>::type;

// MISR<int, 4>::type = PF<MISR<int, 3>::type, 3>::type
// // MISR<int, 3>::type = PF<MISR<int, 2>::type, 2>::type
// // // MISR<int, 2>::type = PF<MISR<int, 1>::type, 1>::type
// // // // MISR<int, 1>::type = IS<int, 0>
// // // MISR<int, 2>::type = PF<IS<int, 0>, 1>::type = IS<1, 0>
// // ...

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

/* ---------- ---------- 打印名字 ---------- ---------- */

int main(void)
{
    IntegerSequence_t<int, 4> tmpobj3;
    std::cout << typeName<decltype(tmpobj3)>() << std::endl;
}