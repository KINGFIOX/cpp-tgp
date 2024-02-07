/**
 * @file p4.cxx
 * @author your name (you@domain.com)
 * @brief 老师的实现版本 make_integer_sequence
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cxxabi.h>
#include <iostream>

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

/* ---------- ---------- push_back ---------- ---------- */

template <typename IS, unsigned int NewElem> // INTSEQ代表整个的std::integer_sequence< ......>类型
struct IntSeq_PushBack; // 因为不使用泛化版本，所以泛化版本可以只声明不定义。

// 向std::integer_sequence末尾插入元素：特化版本
template <typename T, unsigned int... Elems, unsigned int NewElem> // 泛化版本一般要求可变参放在最后，特化版本一般无此要求
struct IntSeq_PushBack<IntegerSequence<T, Elems...>, NewElem> {
    using type = IntegerSequence<T, Elems..., NewElem>;
};

/* ---------- ---------- make_integer_sequence ---------- ---------- */

// 泛化版本
template <typename T, unsigned int N>
struct MakeIntegerSequence // 实现std::make_integer_sequence功能
{
    // 依次遍历出4,3,2,1，应该往末尾插入元素，所以还应该引入IntSeq_PushBack操作。
    using type = typename IntSeq_PushBack<typename MakeIntegerSequence<T, N - 1>::type, N - 1>::type; // 这里理解成函数调用，把type理解成所调用的函数名
};

/**
 * @brief base case
 *
 * @tparam T
 */
template <typename T>
struct MakeIntegerSequence<T, 1> // 这里是1，意味着递归到1就可以了
{
    using type = IntegerSequence<T, 0>; // 这是0，注意。
};

// 定义别名模板
template <typename T, unsigned int N>
using IntegerSequence_t = typename MakeIntegerSequence<T, N>::type;

// MIS<int, 4>::type = PB<MIS<int, 3>::type, 3>::type
// // MIS<int, 3>::type = PB<MIS<int, 2>::type, 2>::type
// // // MIS<int, 2>::type = PB<MIS<int, 1>::type, 1>::type
// // // MIS<int, 1>::type（base case） = IS<int, 0>
// // // MIS<int, 2>::type = PB<IS<int, 0>, 1>::type = IS<int, 0, 1>
// // MIS<int, 3>::type = PB<IS<int, 0, 1>, 2>::type = IS<int, 0, 1, 2>
// MIS<int, 4>::type = PB<IS<int, 0, 1, 2>, 3>::type = IS<int, 0, 1, 2, 3>

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