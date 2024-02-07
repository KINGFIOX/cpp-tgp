/**
 * @file p2.cxx
 * @author your name (you@domain.com)
 * @brief gpt4 生成的 std::make_integer_sequence 的实现
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cxxabi.h>
#include <iostream>

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

/**
 * @brief integer_sequence 实现，方便后面使用
 *
 * @tparam T
 * @tparam Ints
 */
template <typename T, T... Ints>
struct IntegerSequence {
    using value_type = T;
    static constexpr std::size_t size() noexcept { return sizeof...(Ints); }
};

// 辅助模板来递归生成整数序列
template <typename T, std::size_t N, T... Is>
struct make_integer_sequence_helper : make_integer_sequence_helper<T, N - 1, N - 1, Is...> { };

/**
 * @brief base case
 *
 * @tparam T
 * @tparam Is
 */
template <typename T, T... Is>
struct make_integer_sequence_helper<T, 0, Is...> {
    using type = IntegerSequence<T, Is...>;
};

// MIS<int, 5> <==> MISH<int, 5>::type
// MISH<int, 5, 空> : MISH<int, 4, 4, 空>
// // MISH<int, 4, 4> : MISH<int, 3, 3, 4>
// // // MISH<int, 3, 3, 4> : MISH<int, 2, 2, 3, 4>
// // // // MISH<int, 2, 2, 3, 4> : MISH<int, 1, 1, 2, 3, 4>
// // // // // MISH<int, 1, 1, 2, 3, 4> : MISH<int, 0, 0, 1, 2, 3, 4> （MISH<int, 0, IS...>::type = IS<int, 0, 1, 2, 3, 4>）
// type 向上传递，因为是递归

// make_integer_sequence 定义
template <typename T, std::size_t N>
using make_integer_sequence = typename make_integer_sequence_helper<T, N>::type;

int main(void)
{
    // std::make_index_sequence<4> tmpobj2;
    using seq = make_integer_sequence<int, 5>; // 等价于 integer_sequence<int, 0, 1, 2, 3, 4>
}