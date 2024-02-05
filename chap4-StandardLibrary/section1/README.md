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

### 调用引用限定符修饰的 成员函数范例

## 3 - 推导函数返回值范例
