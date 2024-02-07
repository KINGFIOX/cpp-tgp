# chap4_7 - std::integer_sequence

下面是 llvm - clang 的实现：

```cxx
template<class _Tp, _Tp... _Ip>
struct integer_sequence
{
    using value_type = _Tp;
    static_assert( is_integral<_Tp>::value,
                  "std::integer_sequence can only be instantiated with an integral type" );
    static constexpr size_t size() noexcept { return sizeof...(_Ip); }
};
```

用于产生整型数字序列：0 1 2 3 4 5 ...

\_Tp: int, unsigned int, char short

\_Ip: 非类型模板参数报，一堆数字，这堆数字类型相同

std::make_integer_sequence，别名模板，就可以理解成是：一个 std::integer_sequence，
大概感觉就是：

```cxx
template <class T, T N>
using make_integer_sequence = std::integer_sequence<T, 0, 1, 2, 3, ..., N-1>;
```

C++ 标准库还引入了：`std::make_index_sequence`，也是一个别名模板，
大概感觉如下：

```cxx
template <std::sizt_t N>
using make_index_sequence = std::integer_sequence<std::size_t, 0, 1, 2, ..., N - 1>;
```

## 1 - 正向排列数字生成一个类型 Integer_Sequence（实现）

这个有什么用途？后面学 tuple 的时候有用

```cxx
/* ---------- ---------- push_back ---------- ---------- */

template <typename IS, unsigned int NewElem>
struct IntSeq_PushBack;

template <typename T, unsigned int... Elems, unsigned int NewElem>
struct IntSeq_PushBack<IntegerSequence<T, Elems...>, NewElem> {
    using type = IntegerSequence<T, Elems..., NewElem>;
};

/* ---------- ---------- make_integer_sequence ---------- ---------- */

// 泛化版本
template <typename T, unsigned int N>
struct MakeIntegerSequence // 实现std::make_integer_sequence功能
{
    using type = typename IntSeq_PushBack<typename MakeIntegerSequence<T, N - 1>::type, N - 1>::type;
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

int main(void) {
    IntegerSequence_t<int, 4> tmpobj3;
}
```

上面，就是把`::type`看成是一种：函数调用

## 2 - 逆向排列数字生成一个类型 Integer_Sequence_Reverse

那就是来一个 push_front 就行了

```cxx
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
```

## 3 - 将一个数字重复多次生成一个类型 Repeat_Integer

需求：`repeat_sequence<1, 4> = integer_sequence<1, 1, 1, 1>`，
可以使用递归，再来一个计数器，可以实现

前面是通过递归调用`::type`的方式实现，下面使用递归继承的方式展开（gpt 给我生成的几个代码是：递归继承的）

```cxx
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
// 一系列的继承 RI<10, 3, IS<int, 空>>::type = （老祖中有 type）

int main(void)
{
    RepeatInteger_t<10, 4> tmpObj1;
    std::cout << typeName<decltype(tmpObj1)>() << std::endl;
}
```
