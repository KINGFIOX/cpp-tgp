# chap5_2 - 萃取技术中的 value traits（值萃取）演示

## 1 - 一个常规的范例

```cxx
/* ---------- ---------- 固定萃取 ---------- ---------- */

/**
 * @brief 泛化版本，只声明不实现
 *
 * @tparam T
 */
template <typename T>
struct SumFixedTraits;

/**
 * @brief 特化版本
 *
 * @tparam
 */
template <>
struct SumFixedTraits<char> {
    using sumT = int; // 代表返回的类型是 int 类型
};

template <>
struct SumFixedTraits<int> {
    using sumT = int64_t; // 代表返回的类型是 int 类型
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;
    sumT sum {};
    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    int arr1[] = { 10, 15, 20 };
    int arr2[] = { 1000000000, 1500000000, 2000000000 };
    char arr3[] = "abc";

    cout << funcSum(&arr1[0], &arr1[2]) << endl;
    cout << funcSum(&arr2[0], &arr2[2]) << endl;
    cout << funcSum(&arr3[0], &arr3[2]) << endl;
}
```

就是我们之前的这段代码中，有一个`sumT sum{};`。
这个是零初始化，但是，并不是所有类型都适合零初始化的，
如果是一个 类类型，或许就不可以进行零初始化，看 `chap5/comment.md`。

值萃取的核心思想：给进来一个类型，萃取出一个值
（给进来一个东西，不仅仅萃取出一个类型，还可以萃取出一个值）

我们现在就是要尝试：将零初始化 引入到 固定萃取中

```cxx
template <>
struct SumFixedTraits<int> {
    using sumT = int64_t; // 代表返回的类型是 int 类型
    static const sumT initValue = 0; // static const 是在 编译期间 就确定好的
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;

    // sumT sum {};
    sumT sum = SumFixedTraits<T>::initValue; // 给进来一个类型 T，返回一个值。

    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}
```

上面这个看起来还没什么问题。
但是如果给一个 double 类型初始化，可能会有问题：

```cxx
template <typename T>
struct SumFixedTraits;

// FIXME
template <>
struct SumFixedTraits<double> {
    using sumT = double; // 代表返回的类型是 int 类型
    // static const sumT initValue = 0.0;
    /**
     * 错误： in-class initializer for static data member of type 'const sumT' (aka 'const double')
     * 		requires 'constexpr' specifier [-Wstatic-float-init]
     *
     * MSVC： 带有累内初始化表达式的 静态数据成员 必须具有不可变的常量整型类型，或必须被指定为 内联
     */

    // 但是如果使用 C++11 以后的 constexpr 就可以了
    static constexpr sumT initValue = 0.0;

    // inline static constexpr sumT initValue = 0.0; // 同样也可以
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;

    // sumT sum {};
    sumT sum = SumFixedTraits<T>::initValue; // 给进来一个类型 T，返回一个值。

    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    double arr4[] = { 1.8, 2.3, 3.7 };

    cout << funcSum(&arr4[0], &arr4[2]) << endl;
}
```

下面是一个 类类型（非内置类型）

```cxx
class A {
public:
    A(int v1, int v2)
        : m_i(v1 + v2)
    {
    }
    int m_i;
};

// FIXME
template <>
struct SumFixedTraits<A> {
    using sumT = A; // 代表返回的类型是 int 类型
    // static const sumT initValue { 0, 0 };
    static const sumT initValue;
    /**
     * 错误： Static data member of type 'const sumT' (aka 'const A') must be initialized out of line
     *
     * 带有累内初始化表达式的 静态数据成员 必须具有不可变的常量整型类型，或必须被指定为 内联
     */
    // inline static const sumT initValue { 0, 0 }; // 也是可以的 （C++17 以后）
};
A const SumFixedTraits<A>::initValue = A { 0, 0 }; // 有点啰嗦，所以还是 inline 解决的好
```

总结：数组元素类型是 类类型时，初始化 initValue 的方法：

1. 在针对类 A 特化的 SumFixedTraits 类模板中只对 initValue 进行声明，
   然后在某个`.cxx`源文件中对 initValue 进行定义和初始化
2. 采用 C++17 引入的 inline 解决方式，直接在针对 类 A 特化的 SumFixedTraits 类模板中解决问题
3. ++ 在 SumFixedTraits 的特化版本中，引入静态成员函数 initValue(inline) 的方式来解决问题。 ++

下面是 第 3 种方法的演示：

```cxx
class A {
public:
    A(int v1, int v2)
        : m_i(v1 + v2)
    {
    }

public:
    A& operator+=(const A& obj)
    {
        this->m_i += obj.m_i;
        return *this;
    }
    int m_i;
};

/* ---------- ---------- 固定萃取 ---------- ---------- */

/**
 * @brief 泛化版本，只声明不实现
 *
 * @tparam T
 */
template <typename T>
struct SumFixedTraits;

// FIXME
template <>
struct SumFixedTraits<A> {
    using sumT = A;
    static sumT initValue()
    {
        return sumT { 0, 0 };
    }
};

template <typename T>
auto funcSum(const T* begin, const T* end)
{
    using sumT = typename SumFixedTraits<T>::sumT;

    // sumT sum {};
    // sumT sum = SumFixedTraits<T>::initValue; // 给进来一个类型 T，返回一个值。
    sumT sum = SumFixedTraits<T>::initValue();

    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    double arr4[] = { 1.8, 2.3, 3.7 };
    A myAObj[] = { A { 2, 3 }, A { 3, 4 }, A { 5, 6 } };
    cout << funcSum(&arr4[0], &arr4[2]) << endl;
    cout << funcSum(&myAObj[0], &myAObj[2]).m_i << endl;
}
```

## 2 - 判断是否是 void 类型的一个范例

```cxx
int main(void)
{
    cout << std::is_void<int>::value << endl; // 0
    cout << std::is_void<void>::value << endl; // 1
}
```

如何实现的？（个人想法 继承 true_type）

```cxx
template <typename T>
struct voidValueTraits : std::false_type { };

template <>
struct voidValueTraits<void> : std::true_type { };
```

老师的想法：

```cxx
template <typename T>
// struct voidValueTraits : std::false_type { };
struct voidValueTraits {
    static const bool value = false;
};

template <>
// struct voidValueTraits<void> : std::true_type {
struct voidValueTraits<void> {
    static const bool value = true;
};
```

## 3 - 判断两个类型是否相同

`is_same`类模板：用于判断两个类型是否相同

下面是我的想法：

```cxx
/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
struct IsSame : std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam T
 */
template <typename T>
struct IsSame<T, T> : std::true_type { };
```

下面是老师的想法：

```cxx
/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U>
struct IsSame {
    static const bool value = false;
};

/**
 * @brief 特化版本
 *
 * @tparam T
 */
template <typename T>
struct IsSame<T, T> {
    static const bool value = true;
};
```
