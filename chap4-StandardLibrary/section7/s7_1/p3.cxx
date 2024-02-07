/**
 * @file p3.cxx
 * @author your name (you@domain.com)
 * @brief gpt4 生成的 push_back 的版本
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>

// 定义integer_sequence模板，代表一系列整数。
template <int... Ns>
struct integer_sequence {
    using type = integer_sequence;
};

/**
 * @brief 泛化版本，只声明，不定义
 *
 * @tparam Seq
 * @tparam N
 */
template <typename Seq, int N>
struct integer_sequence_push_back;

/**
 * @brief 特化版本
 *
 * @tparam Ns
 * @tparam N
 */
template <int... Ns, int N>
struct integer_sequence_push_back<integer_sequence<Ns...>, N> {
    using type = integer_sequence<Ns..., N>;
};

// 定义 make_integer_sequence 模板，用于生成0到N-1的整数序列。
// 它使用了 integer_sequence_push_back 来在编译时递归地构建序列。
template <int N, int... Ns>
struct make_integer_sequence
    : integer_sequence_push_back<typename make_integer_sequence<N - 1, Ns...>::type, N - 1> { };

// MIS<5, Ns...> : PB<MIS<4, Ns...>::type, 4>
// // MIS<4, Ns...> : PB<MIS<3, Ns...>::type, 3>
// // // MIS<3, Ns...> : PB<MIS<2, Ns...>::type, 2>
// // // // MIS<2, Ns...> : PB<MIS<1, Ns...>::type, 1>
// // // // // MIS<1, Ns...> : PB<MIS<0, Ns...>::type, 0>
// // // // // // MIS<0, Ns...>::type = IS<>
// // // // // MIS<1, Ns...> : PB<IS<>, 0> （也会继承 type = IS< 空, 0> ）
// // // // Mis<2, Ns...> : PB<IS<0>, 1> （type = IS<0, 1>）
// // // MIS<3, Ns...> : PB<IS<0, 1>, 2> （type = IS<0, 1, 2>）
// // MIS<4, Ns...> : PB<IS<0, 1, 2>, 3> （type = IS<0, 1, 2, 3>）
// MIS<5, Ns...> : PB<IS<0, 1, 2, 3>, 4> （type = IS<0, 1, 2, 3, 4>）
// using seq = MIS<5>::type <==> IS<0, 1, 2, 3, 4> （这也是 MIS<N, Ns...>）

/**
 * @brief 递归的 base case
 *
 * @tparam Ns
 */
template <int... Ns>
struct make_integer_sequence<0, Ns...> {
    using type = integer_sequence<Ns...>;
};

// 使用示例
using sequence = make_integer_sequence<5>::type; // 等价于 integer_sequence<0, 1, 2, 3, 4>

int main()
{
    // 使用sequence类型，例如打印类型信息或进行编译时计算
}