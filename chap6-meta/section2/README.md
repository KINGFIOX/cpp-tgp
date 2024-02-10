# chap6_2 - 混合元编程

上一节将：元函数分为了两类：

混合元编程：（三个方面）

1. 程序员写出一段元编程代码
2. 编译器根据这段代码 生成 出一段新代码，实现程序真正功能的是 这段生成的新代码
3. 编译器会对这段新代码进行编译，产生出最终的可执行程序

混合元编程可以看成是：运行期 C++ 代码的生成器

## 1 - 常规的计算 点积 范例

1. 数组 a 有三个元素，`a[0]=1`、`a[1]=2`、`a[2]=3`
2. 数组 b 有三个元素，`b[0]=4`、`b[1]=5`、`b[2]=6`
3. a 和 b 的点积 是一个数值，`a[0]*b[0] + a[1]*b[1] + a[2]*b[2]`

```cxx
template <typename T, int U>
inline auto DotProduct(T* arr1, T* arr2)
{
    T dpResult = T {}; // 零初始化
    for (int i = 0; i < U; ++i) {
        dpResult += arr1[i] * arr2[i];
    }
    return dpResult;
}

// 加了 inline 以后，就没有调用了，是直接被替换了。效率提升了
```

好的解决方案：不采用循环，而是将循环直接展开。

## 2 - 混合元编程 计算 点积

不用循环，就能实现点积的效果，从而达到提高程序整体运行效率的目的。

### 范例演示

```cxx
template <typename T, int U>
struct DotProduct {
    static T result(const T* a, const T* b)
    {
        // 元素个数减少，并且向后跳一步
        return (*a) * (*b) + DotProduct<T, U - 1>::result(a + 1, b + 1);
    }
};

template <typename T>
struct DotProduct<T, 0> {
    static T result(const T* a, const T* b)
    {
        return T {};
    }
};

int main(void) {
    int a[] = { 1, 2, 3 };
    int b[] = { 3, 4, 5 };
    int result = DotProduct<int, 3>::result(a, b);
}
```

因此，上面这段代码实际执行或许是（msvc）：

```cxx
int result = (*a) * (*b) + (*(a+1)) *(*(b+1)) + (*(a+2)) *(*(b+2))
```

### C++ 编译的两个阶段

把整个编译过程 分成两个阶段：前期阶段、后期阶段

1. 前期阶段：此阶段的 C++ 编译器实际上充当 解释器 的角色：
   直接针对程序员开发的 C++ 原发（元编程代码）进行解释执行，
   这一阶段的工作成果就是产生了 一系列的 C++ 代码（所以元编程才被看做 运行期 C++ 代码生成器）
2. 后期阶段：此阶段的 C++ 编译器 恢复了大家比较熟悉的 C++ 编译器本应该具备的功能。
   即：针对前期阶段产生的结果代码进行 编译、链接，最终生成可执行文件

### 范例演示的扩展 —— 代码的智能化产生 与 膨胀

那这个`(*a) * (*b) + (*(a+1)) *(*(b+1)) + (*(a+2)) *(*(b+2)) + ...`
这样的展开是否会有上限呢？

向量的维度是多少，就会有多少个 实例化的 函数。

老师的演示中，12 维度点积，在`DotProduct<int, 12>`中有`call`。
说明 msvc 有智能性，会避免产生冗长（代码膨胀）的 C++ 汇编指令（通过一些函数调用来实现）。

### 类模板实例化 进一步观察

```cxx
template <int x_v, int y_v>
struct InstantiationObServe {
    static const int value = (x_v > y_v)
        ? InstantiationObServe<x_v - 1, y_v>::value
        : InstantiationObServe<x_v, x_v>::value;
};

template <>
struct InstantiationObServe<5, 4> {
    static const int value = (5 > 4) ? InstantiationObServe<4, 4>::value
                                     : InstantiationObServe<5, 5>::value;
};

template <>
struct InstantiationObServe<4, 4> {
    static const int value = 4;
};

template <>
struct InstantiationObServe<5, 5> {
    static const int value = 5;
};

template <>
struct InstantiationObServe<6, 6> {
    static const int value = 6;
};

template <>
struct InstantiationObServe<6, 4> {
    static const int value = (6 > 4) ? InstantiationObServe<5, 4>::value
                                     : InstantiationObServe<6, 6>::value;
};

template <int x_v>
struct InstantiationObServe<x_v, x_v> {
    static const int value = x_v;
};
```

元编程时要小心使用`条件运算符`，我们可以看到：这里`<5, 5>`、`<6, 6>`都有，
实际上应该这么说：元编程的时候，条件运算符，所涉及到的类，两边都会实例化出来。

因此，我刚才出现了一个 bug：

```cxx
template <int x_v, int y_v>
struct InstantiationObServe {
    static const int value = (x_v > y_v) ? InstantiationObServe<x_v - 1, y_v>::value
                                         : InstantiationObServe<x_v, y_v - 1>::value;
    /* 这里刚才就出现一个编译错误：说是递归栈超过 1024 了 */
};
```

有没有方式可以去掉这个特性呢？ 可以使用 `std::conditional`

```cxx
/* 泛化版本 */
template <int x_v, int y_v>
struct InstantiationObServe {
    using TmpType = typename std::conditional<(x_v > y_v),
        InstantiationObServe<x_v - 1, y_v>, InstantiationObServe<x_v, y_v - 1>>::type;

    static const int value = TmpType::value;
};

/* 特化版本 */
template <int x_v>
struct InstantiationObServe<x_v, x_v> {
    static const int value = x_v;
};
```

#### 静态成员改成枚举类型

当然，如果觉得`static const int value`会占用内存的话，可以改成枚举类型

```cxx
template <int x_v, int y_v>
struct InstantiationObServe {
    using TmpType = typename std::conditional<(x_v > y_v),
        InstantiationObServe<x_v - 1, y_v>, InstantiationObServe<x_v, y_v - 1>>::type;

    // static const int value = TmpType::value;
    enum { value = TmpType::value };
};

template <int x_v>
struct InstantiationObServe<x_v, x_v> {
    // static const int value = x_v;
    enum { value = x_v };
};
```

#### 元编程：“计算完整性”

元编程：“计算完整性” 概念，包含几点：

1. 状态变量，指的是：InstantiationObServe 类模板中的模板参数
2. 迭代构造（循环构造）：通过在 InstantiationObServe 模板中融入递归编程技术
3. 执行路径的选择：使用条件运算符（或者是 `std::conditiaonal`）以及 InstantiationObServe 类模板的特化
   （达到递归结束的效果），当然也可以调用 其他元函数
4. 整数对象（整数运算）：静态成员变量 value（或者枚举类型孩子 value），其他功能等价于 变量（常量）

元编程中不能使用变量（编译期间能接受的只有静态常量），
传统意义上的分支、循环在元编程中只能通过：条件运算符、特化、递归等手段来实现。
所以这种编程风格常常也被称为：函数式编程

### inline 静态成员变量

inline 关键字：建议编译器，将函数调用替换成 函数本体，提高运行效率（省去调用函数的开销）。

类中的 静态成员变量。class 一般是在 某个 .hpp 中声明，然后在某个 .cxx 中初始化（分配内存）静态成员变量。

如果没有在某个 .cxx 中初始化（一个无法解析的外部符号，链接错误）；
如果在多个 .cxx 中初始化了（重定义）

C++11 以后，支持了 静态成员变量的初始化。（也就是下面这种 `=` ）

```cxx
struct TST {
    static const int myStatic = 1;
    /* 但是这种初始化方式有点问题：只能是 int 相关的类型：short、long、char */
    /* 其次必须用 const 修饰 */
    /* 但是其实也是有问题的，这样操作的后果：静态成员变量的生命周期是 覆盖整个程序运行时 */
};
```

下面是 inline 的实现方法（C++17 以后）

```cxx
struct TST {
    // static inline int myStatic = 1;
    static inline double myStatic = 1;
    /* 如果在代码中没有用到这个类，这个静态成员 就不会初始化 */
    /* 而且这个还支持 double */
};
```

当然，也可以使用 constexpr（C++11） 实现；
constexpr 自带 inline 属性（C++17）

```cxx
struct TST {
    static constexpr double myStatic = 1;
    /* 当然这种写法就不能修改变量了 */
};
```

有一个有意思的问题：inline const 是否等价于 constexpr ？
