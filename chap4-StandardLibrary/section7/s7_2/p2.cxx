/**
 * @file p2.cxx
 * @author your name (you@domain.com)
 * @brief repeat_sequence
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstddef>
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

/**
 * @brief 泛化版本
 *
 * @tparam Num 重复的数字的名称
 * @tparam RepeatTime 要重复的次数
 * @tparam IS
 */
template <std::size_t Num, std::size_t RepeatTime, typename IS = IntegerSequence<std::size_t>>
class RepeatInteger;

/**
 * @brief 特化版本 （递归）
 *
 * @tparam Num
 * @tparam RepeatTime 继承的时候，每次 - 1
 * @tparam index
 */
template <std::size_t Num, std::size_t RepeatTime, std::size_t... index>
class RepeatInteger<Num, RepeatTime, IntegerSequence<std::size_t, index...>>
    : public RepeatInteger<Num, RepeatTime - 1, IntegerSequence<std::size_t, index..., Num>> {
    /* 继承的时候，repeat--，然后 IntegerSequence 中多了一个 Num */
};

/**
 * @brief base case
 *
 */
template <std::size_t Num, std::size_t... index>
class RepeatInteger<Num, 0, IntegerSequence<std::size_t, index...>> {
public:
    using type = IntegerSequence<std::size_t, index...>;
};

template <std::size_t Num, std::size_t RepeatTime>
using RepeatInteger_t = RepeatInteger<Num, RepeatTime>::type;

// RepeatInteger<10, 4>::type
// RI<10, 4, IS<int, 空>> : RI<10, 3, IS<int, 10>>
// // RI<10, 3, IS<int, 10>> : RI<10, 2, IS<int, 10, 10>>
// // // RI<10, 2, IS<int, 10, 10>> : RI<10, 1, IS<int, 10, 10, 10>>
// // // // RI<10, 2, IS<int, 10, 10, 10> : RI<10, 0, IS<int, 10, 10, 10, 10>>
// // // // // RI<10, 0, IS<int, 10, 10, 10, 10>::type = IS<int, 10, 10, 10, 10>
// 一系列的继承 RI<10, 3, IS<int, 空>>::type =

/* ---------- ---------- 打印名字 ---------- ---------- */

std::string
demangle(const char* mangledName)
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
    RepeatInteger_t<10, 4> tmpObj1;
    std::cout << typeName<decltype(tmpObj1)>() << std::endl;
}