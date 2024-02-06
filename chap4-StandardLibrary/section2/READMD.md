# chap4_2 - true_type、false_type

true_type、false_type 是类型别名

```cxx
// integral 整型，constant 常量
typedef integral_constant<bool, true>  true_type;
typedef integral_constant<bool, false> false_type;

template <class _Tp, _Tp __v>
struct _LIBCPP_TEMPLATE_VIS integral_constant
{
  static _LIBCPP_CONSTEXPR const _Tp      value = __v;
  typedef _Tp               value_type;
  typedef integral_constant type;
  _LIBCPP_INLINE_VISIBILITY
  _LIBCPP_CONSTEXPR operator value_type() const _NOEXCEPT {return value;}
#if _LIBCPP_STD_VER > 11
  _LIBCPP_INLINE_VISIBILITY
  constexpr value_type operator ()() const _NOEXCEPT {return value;}
#endif
};
```

注意区分：true_type、false_type 与 true、false 的区别

true_type、false_type 代表类型（类模板），
true、false 代表值

也就是：

```cxx
true_type func();
```

而不能

```cxx
true func();
```

注意：bool 可以代表 true，也可以代表 false。
而 true_type 类型代表的是 true；false_type 类型代表的是 false

## 手动实现 true_type、false_type

```cxx
template <bool val>
struct BoolConstant {
    using type = BoolConstant; /* <==> using type = BoolConstant<val>; 也行 */
    static constexpr bool value = val; /* 标准库源码中也通常会有这一行 */
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;
```

记住几点：

1. TrueType 和 FalseType 代表一个类类型，TrueType 代表一种 true（真）的含义；
   而 FalseType 代表一种 false（假）的含义

2. 用法：一般当做基类被继承使用。当作为基类被继承时，派生类也就具备了 true 或 false 的这种意味。
   比如标准库中，is，比如说 is_pointer、is_union、is_function 等类模板，
   都是继承 `BoolConstant<true>` 或者 `BoolConstant<false>`

   ```cxx
   template<class _Tp>
   struct _LIBCPP_TEMPLATE_VIS is_pointer : _BoolConstant<__is_pointer(_Tp)> { };
   ```

3. 可以当做一种返回类型被使用，例如：

```cxx
template <typename T, bool val>
struct AClass {
    AClass()
    {
        cout << "AClass:AClass() 执行了!" << endl;
        if (val) {
            //
            T tmpa = 15;
            /* 错误：No viable conversion from 'int' to */
        } else {
            //
            T tmpa = "abc";
            /* 错误：Cannot initialize a variable of type 'int' with an lvalue of type 'const char[4]' */
        }
    }
};

int main(void)
{
    AClass<int, true> a;
    AClass<string, false> b;
}
```

上面这段代码，分开看是正确的，如果 `a` 只走 true 分支，`b` 只走 false 分支，
但是这两段代码放在一起就 Boom! 了，报错信息如上。

当然如果是将：`if` 换成 `if constexpr` 就没啥问题。

编译器虽然可以在编译时期判断出来 if-else 分支，但是实际上，两个都会被编译

### 使用 true_type、false_type

```cxx
temtemplate <bool val>
struct BoolConstant {
    using type = BoolConstant; /* <==> using type = BoolConstant<val>; 也行 */
    static constexpr bool value = val; /* 标准库源码中也通常会有这一行 */
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;
plate <typename T, bool val>
struct AClass {
    AClass()
    {
        cout << "AClass:AClass() 执行了!" << endl;
        // if constexpr /* ✅ */ (val) {
        // if (val) {
        //     //
        //     T tmpa = 15;
        //     /* 错误：No viable conversion from 'int' to */
        // } else {
        //     //
        //     T tmpa = "abc";
        //     /* 错误：Cannot initialize a variable of type 'int' with an lvalue of type 'const char[4]' */
        // }

        ACLassEX(BoolConstant<val>());
    }

    void ACLassEX(TrueType abc)
    {
        T tmpa = 15;
    }

    void ACLassEX(FalseType abc)
    {
        T tmpa = "abc";
    }
};

int main(void)
{
    AClass<int, true> a;
    AClass<string, false> b;
}
```
