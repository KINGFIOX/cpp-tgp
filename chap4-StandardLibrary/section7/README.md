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

int main(void) {
    IntegerSequence_t<int, 4> tmpobj3;
}
```

上面，就是把`::type`看成是一种：函数调用

## 2 - 逆向排列数字生成一个类型 Integer_Sequence_Reverse

## 3 - 将一个数字重复多次生成一个类型 Repeat_Integer
