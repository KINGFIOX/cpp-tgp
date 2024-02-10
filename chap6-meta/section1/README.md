# chap6_1 - 术语、元编程

元编程的目的：将各种计算：从运行期 提前到 编译期进行，以达到提供程序运行时能提升的目的。
是一种 增加程序编译时间，从而提升程序运行效率的一种编程技术。

元编程：会涉及到很多与循环有关的代码，很多操作针对的是：类型、常量。此种循环的实现，往往会采用递归手段

范例：typelist（类型列表）、tuple（元组）

boost 库中，有一个叫做 MPL（meta programming library）库，
这个库是用于：辅助模板元编程。

相关资料：

1. 《C++模板元编程》，对 MPL 有比较详细的介绍（比较老了，因为要模拟可变参模板）。
2. MPL11，使用了 C++11 新标准语法 对 boost 中的 MPL 进行了改写

## 1 - 术语

元函数：meta programming，也叫：模板元编程（template meta programming），
可以理解为一种编程手法，用来实现一些比较特殊的功能

一般和 “递归” 这个词有比较密切的联系。多数都会在元编程中 使用到递归编程技术

元编程随着 C++ 新标准的推出，也在不断发展，能力越来越强大。

模板编程主要应用在两个方面：

1. 泛型编程（generic programming）： “通用”，程序员不需要关心具体类型
2. 元编程（meta programming）：突出一种程序设计技巧，达到常规编程难以达到的效果。
   前面章节：`std::remove_all_extents`类模板的实现。
   这种技巧注重模板在实例化过程中的 一些推导过程。而这些推导过程恰恰是解决问题 和 体现程序设计技巧的过程
   此外，元编程手法还可以让某些计算在程序的编译期间完成

其实可以：将元编程直接理解为泛型编程。元编程只是在编程方式上比传统的泛型编程更特殊一点

## 2 - 元函数

#### 一段示例

```cxx
constexpr int myfunc(int abc)
{
    return abc * 2;
}

int main(void)
{
    constexpr int var = 11 * myfunc(12);
    /* 静态断言，编译期间断言 */
    static_assert(var == 264, "std error");

    return 0;
}
```

#### static_assert 编译期断言

如果是 `int var = 11 * myfunc(12)`，那么在`static_assert`就会出现错误。

```cxx
constexpr int myfunc(int abc)
{
    return abc * 2;
}

int main(void)
{
    int var = 11 * myfunc(12);
    /* 静态断言，编译期间断言 */
    static_assert(var == 264, "std error");
    /* 错误： p1.cxx:12:19: error: static assertion expression is not an integral constant expression */
}
```

#### 给 constexpr 传入 动态变量

如果给`constexpr int var`赋值一个动态（`auto`，c 语言的 auto）的变量，
那么也会出现报错

```cxx
int main(void) {
    int i = 64;
    constexpr int var = 11 * i;
}
```

#### 调用 constexpr 函数

下面这个没有问题：（运行期间调用 constexpr 修饰的函数）

```cxx
constexpr int myfunc(int abc)
{
    return abc * 2;
}

int main(void)
{
    int i = 64;
    int var = 11 * myfunc(i);
}
```

有一个典型的错误，叫做：表达式计算结果不是常数

constexpr 自带 inline 属性，该属性是在 C++17 中引入的。

有了 inline 属性就不会加入到符号表了

### 数值 元函数

1. 编译期间能够被调用的类模板
   范例：在编译期间计算 5 的阶乘： `1 * 2 * 3 * 4 * 5 = 120`
2. constexpr 修饰的函数
3. constexpr 修饰的 变量模板（C++17 之后的版本）

```cxx
template <int n_v>
struct Factorial {
    enum {
        value = n_v * Factorial<n_v - 1>::value
    };
};

/**
 * @brief 递归 base case
 *
 * @tparam
 */
template <>
struct Factorial<1> {
    enum {
        value = 1
    };
};

int main(void)
{
    constexpr int var = Factorial<5>::value;
    static_assert(var == 120, "factorial error");
    std::cout << var << std::endl;
}
```

如果没有设置 base case:

```cxx
错误： recursive template instantiation exceeded maximum depth of 1024
```

下面是改进：

```cxx
constexpr int Factorial(int n_v)
{
    return n_v <= 1 ? 1 : (n_v * Factorial(n_v - 1));
}
```

下面是改进：

```cxx
constexpr int Factorial(int n_v)
{
    int result = 1;
    for (int i = 1; i <= n_v; i++) {
        result *= i;
    }
    return result;
}
```

需要注意的是：`constexpr`修饰的成员函数，依然会有符号表与汇编代码（老师通过 msvc 演示的），
因此依然会有入栈出栈，因此依然有开销。

#### constexpr 修饰的 变量模板

```cxx
/* 泛化版本 */
template <int Arg>
constexpr int result = Arg * result<Arg - 1>;

/* 特化版本 */
template <>
constexpr int result<0> = 1;

int main(void)
{
    std::cout << result<5> << std::endl;
}
```

但是这个`result<5>`依然还可以成为：元函数的，
但是这个长得不太像函数（没有调用的样子）。因此还改成有：函数调用痕迹的

```cxx
template <int Arg>
constexpr int result()
{
    return Arg * result<Arg - 1>();
}

template <>
constexpr int result<0>()
{
    return 1;
}
```

#### 求和的例子

```cxx
/**
 * @brief 泛化版本。
 * 		这个比较有意思，这个 泛化版本 也是 base case，相较于上面
 * @tparam FTArgs
 */
template <int... FTArgs>
constexpr int resultSum = 0;

/**
 * @brief 特化版本
 *
 * @tparam First
 * @tparam Others
 */
template <int First, int... Others>
constexpr int resultSum<First, Others...> = First + resultSum<Others...>;
```

#### 求和的例子（折叠表达式）

```cxx
/**
 * @brief 折叠表达式
 *
 * @tparam FTArgs
 * @return constexpr int
 */
template <int... FTArgs>
constexpr int resultSum()
{
    return (... + FTArgs);
}
```

### 类型 元函数

之前我们介绍过`std::remove_all_extents`类模板。
`remove_all_extents`类模板所实现的元编程就是靠递归模板实例化实现的

```cxx
template <typename T>
struct AddPoint {
    using type = T*;
};
```

像 AddPoint 这种包含了 using 来定义类型别名的类模板，
就可以称为 类型元函数（属于：固定萃取类模板）。

这个可以简化为：变量模板

```cxx
template <typename T>
using AddPoint = T*;
```

这个依然可以称之为：元函数。

元函数是可以比较宽泛的概念，上面有提到：能在程序编译期间被调用 或 执行的函数
