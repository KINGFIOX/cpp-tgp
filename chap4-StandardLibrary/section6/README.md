# chap4_6 - std::remove_all_extents

是 C++11 以后引入的类模板。能力是：把一个数组中的数组类型部分移除掉，只保留一个元素类型

`int a[12]`，这里 a 的类型是`int [12]`，经过 std::remove_all_extents 就是：剔除`[12]`，保留`int`（元素类型）

```cxx
int main(void) {
    double c[2][8][9]; // c 的类型是 double[2][8][9]
    cout << typeid(decltype(c)).name() << endl; // A2_A8_A9_d
    cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << endl; // d
}
```

下面是 llvm - clang 的实现代码：

```cxx
template <class _Tp>  // base case
struct remove_all_extents {
    using type = _Tp;
};

template <class _Tp>
struct remove_all_extents<_Tp[]> { // _Tp[] 表示无边界数组
    using type = typename remove_all_extents<_Tp>::type;
};

template <class _Tp, size_t _Np>
struct remove_all_extents<_Tp[_Np]> { // _Tp[_Np]
    using type = typename remove_all_extents<_Tp>::type;
};

template <class _Tp>
using __remove_all_extents_t = typename remove_all_extents<_Tp>::type;
```

上面这里其实递归了。

什么时候会出现：无边界数组呢？比方说`a.cpp`中定义了`int g_array[10]`全局数组；
那么其他源文件中就会有`extern int g_array[]`（外部变量说明的时候，可以不写数字，无边界数组，类型为`int[]`）。
甚至还可以骗编译器，没有在源文件中定义`int g_array[10]`，但是`extern int g_array[]`了，
这个`remove_all_extents`依然可以得到元素的类型

```cxx
template <class _Tp, size_t _Np>
struct remove_all_extents<_Tp[_Np]> { // _Tp[_Np]
    using type = typename remove_all_extents<_Tp>::type;
};
```

这里就是元编程了：多数情况下都会涉及到递归

```cxx
int main(void) {
    cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << endl; // d
    // 这里把 remove_all_extents<decltype(c)>::type 理解成函数调用就行了
    // 这个写法，编译器就实例化出来了 remove_all_extents<int[2][8][9]>
    //      这里满足：remove_all_extents<_Tp[_Np]> 这个特化版本了
    // 然后这里又：<int[2][8][9]>::type = <int[8][9]>::type
    // <int[8][9]>::type = <int[9]>::type
    // <int[9]>::type = <int>::type = int
    // <int>::type 就进入了 base case 中了： struct remove_all_extents { using type = _Tp; };
}
```

总结：remove_all_extents 类模板的实现用到了 递归模板实例化 的技术手段

## 手动实现，手动调用，insight++

```cxx
template <class Tp>
struct rae {
    using type = Tp;
};

template <class Tp>
struct rae<Tp[]> {
    using type = typename rae<Tp>::type;
};

template <class Tp, size_t Np>
struct rae<Tp[Np]> {
    using type = typename rae<Tp>::type;
};

int main(void)
{
    double c[2][8][9]; // c 的类型是 double[2][8][9]

    cout << typeid(rae<decltype(c)>::type).name() << endl; // d
}
```

C++ insight 得到的结果：

```cxx
template <>
struct rae<double[8][9]> {
    using type = typename rae<double[9]>::type;
};

template <>
struct rae<double[9]> {
    using type = typename rae<double>::type;
};

template <>
struct rae<double> {
    using type = double;
};

template <>
struct rae<double[2][8][9]> {
    using type = typename rae<double[8][9]>::type;
};
```
