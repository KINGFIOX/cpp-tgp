# chap4_8 - std::is_union、std::is_class、std::integral_constant

## 1 - std::is_union

是 C++11 以后出现的：类模板。用于判断某个类型是否是一个 union 类型

```cxx
struct A { };

union B { };

int main(void)
{
    cout << std::is_union<A>::value << endl; // 0
    cout << std::is_union<B>::value << endl; // 1
}
```

这个还是需要编译器内部的东西

C++14 以后：

```cxx
#if _LIBCPP_STD_VER > 14
template <class _Tp>
inline constexpr bool is_union_v = __is_union(_Tp);
#endif
```

## 2 - std::is_class

```cxx
struct A { };

union B { };

int main(void) {
    cout << std::is_class_v<A> << endl; // 1
    cout << std::is_class_v<B> << endl; // 0
}
```

后面会讲`is_class`如何实现的

## 3 - std::integral_constant

是 C++11 引入的：类模板

下面是 llvm - clang 实现的代码

```cxx
template <class T, T val>
struct integral_constant {
    static const T value = val;
    using value_type = T;
    using type = integral_constant;
    inline constexpr operator value_type() const noexcept { return value; }
    inline constexpr value_type operator()() const _NOEXCEPT { return value; }
};

int main(void) {
    std::integral_constant<bool, std::is_union<B>::value> myObj1;
    std::cout << myObj1.value << std::endl; // 1

    std::integral_constant<bool, std::is_union<A>::value> myObj2;
    std::cout << myObj2.value << std::endl; // 0
}
```

上面这个`std::is_union<B>::value`肯定是要比`std::integral_constant<bool, std::is_union<A>::value>::value`简洁的

integral_constant 是一个包装 目的的类，
把`std::is_union<B>::value`包装成了`std::integral_constant`这个类型。
那么在很多需要用到类型的场合（比如函数返回值类型中）就可以使用这个类型。
