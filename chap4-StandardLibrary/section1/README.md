# chap4_1 - std::declval

第四章节：介绍 C++ 标准库中典型且常用的函数模板、类模板、别名模板等功能组件以及他们的实现细节

## 1 - 基本概念和常规范例（std::declval + decltype）

`std::declval`（C++11 以后），没有函数体（只有声明，没有实现），
无法被调用，一般用于与 decltype，sizeof 等关键字配合使用，
来进行类型推导、占用内存空间计算等

下面是 llvm clang++ 的实现版本，和 msvc 不同，我也不懂。

```cxx
template <class _Tp>
_Tp __declval(long);  /* 声明 */

template <class _Tp>
decltype(std::__declval<_Tp>(0)) declval() _NOEXCEPT;
```

下面是 msvc 的实现版本

```cxx
template <class _Tp>
struct add_rvalue_reference {
    using type = __add_rvalue_reference_t<_Tp>::_Rvalue;
};

template<Class _Ty>
using add_rvalue_reference_t = typename add_rvalue_reference<_Ty>::_Rvalue;

template<class _Ty>
add_rvalue_reference_t<_Ty> declval() noexcept;
```

add_rvalue_reference：是 C++ 标准库中的类模板，他的能力是给进来一个类型，他能够返回改类型的右值引用类型。
比如：

1. 给进来一个 int 类型，返回的就是 int&&
2. 给进来一个 int& 类型，返回的还是 int& 类型。这里用到了引用折叠`int& && => int&`（看 chap3 的 comment）
3. 给进来一个 int&& 类型，返回的还是 int&&

std::declval 的功能：返回某一个类型 T 的右值引用，不管该类型是否有默认构造函数，
也不管该类型是否可以创建对象（抽象类不能创建对象）。（就是这么神奇）
这个动作是在编译时完成的，所以很多人把 std::declval 也称为：编译时工具

> decl 是 declare（声明）的简写

```cxx
class A {
public:
    A(int i) /* 构造函数 */
    {
        printf("A::A() 函数执行了, this=%p\n", this);
    }
    double myfunc()
    {
        printf("A::myfunc() 函数执行了, this=%p\n", this);
        return 12.1;
    }
};

int main(void)
{
    using YT = decltype(std::declval<A>()); /* 这个圆括号不能缺了，缺了，不报错，但是含义完全变了 */

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<YT>().pretty_name() << endl; // T=A&&

    cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(
        declval<A>().myfunc()
    )>().pretty_name() << endl; // myaobj.myfunc() 的返回类型=double
}
```

### 不求值表达式 decltype 的好处

```cxx
A&& aYinObj(); /* 声明，但是没有实现 */
/**
 * 该函数返回类型是 A&&，可以看成是一返回了一个 A&& 类型的对象，这种对象就可以看成是 类 A 对象。
 * 这种对象去调用 myfunc 的写法就没有错误 ---> aYinObj.myfunc(); 语法上也没啥毛病
 * decltype(aYinObj().myfunc()) myDBLvar; // 定义了一个 double 类型的 左值变量（编译链接都不报错）
 * 		decltype 推断的时候，不需要调用函数（不求值表达式）
 */

int main(void)
{
    A myaobj(10); // 创建对象
    cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(myaobj.myfunc())>().pretty_name() << endl; // myaobj.myfunc() 的返回类型=double
    /**
     * 观察现象，得到两条结论：
     * 1. 要得到 myfunc() 的返回类型，有些麻烦：要创建对象，创建对象的话还要传参，并且还要调用对象，尽管 decltype 不求值
     * 2. 当然，如果是私有函数，那么 decltype(myaobj.myfunc()) 就不允许了
     */
}
```

### std::declval 的作用

1. 从类型转换的角度来讲：将任意一个类型转换成 右值引用类型
2. 假想创建出某类型对象的角度来说，配合 decltype，不必经过该类型的构造函数 就可以使用该类型的成员函数

std::declval 不能被调用，也不能创建任何对象。但 std::declval 能在不创建对象的情况下，达到：
创建一个该类型对象的效果 或者说 程序员可以假定创建出了一个该类型对象

## 2 - std::declval 为什么返回右值引用类型

### 返回类型本身是不好的（语义限制）

返回原始类型，语义上有创建临时对象的意思。

因为返回 原始类型，导致为了 遵循语义限制，编译器内部创建了临时的 A 类 对象。

为了绕开语义限制，在设计 decltype() 函数模板时，就不要返回类型 T 了。
可以返回 T&，也可以返回 T&&。这样从 遵循语义限制 方面来说，就不会创建临时的 A 类 对象了。

注意：上面 "编译器内部创建临时 A 类对象"，也就是所说的：假想创建了对象。

```cxx
class A {
public:
    A()
    {
        printf("A::A() 函数执行了, this=%p\n", this);
    }
    A(int i) /* 构造函数 */
    {
        printf("A::A(int) 函数执行了, this=%p\n", this);
    }
    double myfunc()
    {
        printf("A::myfunc() 函数执行了, this=%p\n", this);
        return 12.1;
    }

private:
    ~A() { }
};

template <typename T>
T mydeclval() noexcept; /* 我们自己模拟一个 declval，但是是原始类型 */

int main(void)
{
    using boost::typeindex::type_id_with_cvr;

    cout << "myaobj 的返回类型=" << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name() << endl;
    // 这里并不是要调用 myfunc，因此语义上没有要创建一个 临时对象

    // cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(mydeclval<A>().myfunc())>().pretty_name() << endl;
    // 虽然不是创建一个 真实的对象，mydeclval<A>() 看起来是要作为一个操作数，参与计算。因此要遵循一个 语义限制
    // 这里的语义是：创建一个临时对象
    // 返回原始类型，就是有一种 创建临时对象的语义（将亡值）
    // 与 decltype 类似的，还有 sizeof
    /**
     * 错误：
     * error: s1_3.cxx:47:77: error: temporary of type 'A' has private destructor
     * 47 |     cout << "myaobj.myfunc() 的返回类型=" << type_id_with_cvr<decltype(mydeclval<A>().myfunc())>().pretty_name() << endl;
     * |                                                                        ^
     * s1_3.cxx:35:5: note: declared private here
     * 35 |     ~A() { }
     * |     ^
     *
     */
}
```

### 返回左值引用? 还是 返回右值引用?

#### 左值 （引用折叠）

```cxx
template <typename T>
T& mydeclval() noexcept; /* 我们自己模拟一个 declval，但是 左值引用 */

int main(void)
{
    using boost::typeindex::type_id_with_cvr;

    cout << "mydeclval<A> => " << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name() << endl;
    cout << "mydeclval<A&> => " << type_id_with_cvr<decltype(mydeclval<A&>())>().pretty_name() << endl;
    cout << "mydeclval<A&&> => " << type_id_with_cvr<decltype(mydeclval<A&&>())>().pretty_name() << endl;
    /**
     * 下面三个相同，引用折叠
     * mydeclval<A> => A&
     * mydeclval<A&> => A&
     * mydeclval<A&&> => A&
     */
}
```

#### 右值

返回右值，实际得到的类型更全面

```cxx
template <typename T>
T&& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

int main(void)
{
    using boost::typeindex::type_id_with_cvr;

    cout << "mydeclval<A> => " << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name() << endl;
    cout << "mydeclval<A&> => " << type_id_with_cvr<decltype(mydeclval<A&>())>().pretty_name() << endl;
    cout << "mydeclval<A&&> => " << type_id_with_cvr<decltype(mydeclval<A&&>())>().pretty_name() << endl;
    /**
     * 下面三个相同，引用折叠
     * mydeclval<A> => A&&
     * mydeclval<A&> => A&
     * mydeclval<A&&> => A&&
     */
}
```

### 调用引用限定符修饰的 成员函数范例

```cxx
class ALR {
public:
    void onAnyValue()
    {
        cout << "ALR::onAnyValue() 函数执行了!" << endl;
    }
    void onLValue() & /* 限定符 &，只能被左值对象调用 */
    {
        cout << "ALR::onLValue() 函数执行了!" << endl;
    }
    // void onLvalue() && /* 限定符 &&，只能被右值对象调用 */ // 这种重载是 ✅ 的
    // {
    //     cout << "ALR::onAnyValue() 函数执行了!" << endl;
    // }

    void onRValue() &&
    {
        cout << "ALR::onRValue() 函数执行了!" << endl;
    }
};

#define R 1

#if R

template <typename T>
T&& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

#else

template <typename T>
T& mydeclval() noexcept; /* 我们自己模拟一个 declval，右值引用 */

#endif

int main(void)
{
    ALR alr; /* 左值类型 */
    alr.onAnyValue();

    /**
     * alr.onRValue();
     * 错误：'this' argument to member function 'onRValue' is an lvalue,
     *   but function has rvalue ref-qualifier
     */

    ALR().onRValue();

    /**
     * ALR().onLValue();
     * 'this' argument to member function 'onLValue' is an rvalue,
     *	  but function has non-const lvalue ref-qualifier
     *
     */

#if R

    decltype(mydeclval<ALR>().onAnyValue());
    decltype(mydeclval<ALR&>().onLValue()); // 此时 mydeclval -> ALR &
    // decltype(mydeclval<ALR&>().onRValue()); // 错误
    decltype(mydeclval<ALR&&>().onRValue()); // 此时 mydeclval -> ALR &&
    // decltype(mydeclval<ALR&&>().onLValue()); // 错误

#else

    /* mydeclval 返回左值引用，凡是 onRValue() 就是错误的 */

    decltype(mydeclval<ALR>().onAnyValue());
    decltype(mydeclval<ALR&>().onLValue()); // 此时 mydeclval -> ALR &
    // decltype(mydeclval<ALR&>().onRValue()); // 错误
    // decltype(mydeclval<ALR&&>().onRValue()); // 此时 mydeclval -> ALR &&
    decltype(mydeclval<ALR&&>().onLValue()); // ✅

#endif
}
```

## 3 - 推导函数返回值范例

函数类型一般由：函数返回值 和 参数类类型决定，与函数名、形参名没有关系。

### 第一种写法

看 s1_2/s1_2.cxx，感觉这个东西有点玄学

```cxx
template <typename T_F, typename... U_Args>
decltype(std::declval<T_F>()(std::declval<U_Args>()...)) TestFnRtnImpl(T_F func, U_Args... args)
/* std::declval<> 中的 U_Args 写成 U_Args&、U_Args&& 都一样 */
/* std::declval<T_F>()(std::declval<U_Args>()...) <== 根据函数类型、函数参数类型 推导函数的返回值 */
/* std::declval<U_Args>() 创建假想对象，然后... */
/* std::declval<T_F>() ---> int(* &&)(int, int) ==> 可以简单理解成是 int(*)(int, int) */
/* (std::declval<U_Args>()...) 这种写法，推导出来的是 两个 int&& */
/* 这里实际上是假想的调用了 */
{
    auto rtnValue = func(args...);
    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(std::declval<T_F>())>().pretty_name() << endl; // int (*&&)(int, int)
    return rtnValue;
}

int main(void)
{
    auto result = TestFnRtnImpl(myfunc, 89, 64);
    /**
     * T_F 被推断成 int(*)(int, int)
     * U_Args : int, int
     */
    cout << result << endl;
    // decltype(int); // 错误：expected '(' for function-style cast or type construction
    // decltype 里面都是跟着：对象，并不是类，因此不能：
    // decltype(T_F(U_ARGS...))
}
```

### 第二种写法（返回类型后置语法）

返回类型后置语法

```cxx
template <typename T_F, typename... U_Args>
auto TestFnRtnImpl(T_F func, U_Args... args) -> decltype(func(args...))
{
    auto rtnValue = func(args...);
    return rtnValue;
}

int main(void)
{
    auto result = TestFnRtnImpl(myfunc, 89, 64);
    /**
     * T_F 被推断成 int(*)(int, int)
     * U_Args : int, int
     */
    cout << result << endl;
    // decltype(int); // 错误：expected '(' for function-style cast or type construction
    // decltype 里面都是跟着：对象，并不是类，因此不能：
    // decltype(T_F(U_ARGS...))
}
```

并不能去掉 返回类型后置语法`-> decltype(func(args...))`，

有了` -> decltype(func(args...))`后置语法，前面的 auto 就相当于是一种装饰（填充），
实际起作用的是后面的。

不能`auto TestFnRtnImpl(T_F func, U_Args... args)`，
这玩意儿就是 auto 了（形参是 传值，看 chap3-s2），会抛弃`&`或者是`const`等
