# chap4_3 - void_t

## 1 - 源码分析和常规范例

```cxx
#if _LIBCPP_STD_VER > 14
template <class...> using void_t = void;
#endif
```

也就是不管给进去什么东西，都会变成 void

功能：能够检测到 应用 SFINAE 特性时出现的非法类型，
换句话来说：给进来的类型必须是一个有效类型，不能是一个非法类型

### 判断类中是否存在 某个类型别名

```cxx
struct NoInnerType {
    /* 没有类型别名 */
    int m_i;
};

struct HaveInnerType {
    using type = int; /* 类型别名 */
    void myfunc() { }
};

/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct HasTypeMem : std::false_type /* struct 默认是 public 继承；class 默认是 private 继承 */ {
};

/* 特化版本 */
template <typename T>
struct HasTypeMem<T, std::void_t<typename T::type>> : std::true_type {
};

int main(void)
{
    /* 这个 value 来源于继承 */
    /* 并且我们当前的代码，只能识别 public 修饰的，不能识别 private 修饰的 */
    cout << HasTypeMem<NoInnerType>::value << endl; // 0
    /* 没有 type，那么 特化版本就会被忽略 SFINAE，于是使用泛化版本，又泛化版本继承于 false_type，其 value = 0 */
    cout << HasTypeMem<HaveInnerType>::value << endl; // 1
}
```

但是这有不好的，就是：上面这个 HasTypeMem 只能用来判断是否有 type，这写死了，不好。
可以使用宏（macro）来解决

#### 宏

```cxx
struct NoInnerType {
    /* 没有类型别名 */
    int m_i;
};

struct HaveInnerType {
    using type = int; /* 类型别名 */
    void myfunc() { }
};

#define HAS_TYPE_MEM(parMTpNm)                        \
    template <typename T, typename U = std::void_t<>> \
    struct HTM_##parMTpNm : std::false_type { };      \
    template <typename T>                             \
    struct HTM_##parMTpNm<T, std::void_t<typename T::parMTpNm>> : std::true_type { };
/* ##parMTpNm 这里的 ##parMTpNm 解引用 + 字符串连接 */

/* 使用宏定义：HTM_type */
HAS_TYPE_MEM(type);

int main(void)
{
    cout << HTM_type<HaveInnerType>::value << endl;
}
```

### 判断类中是否存在 某个成员变量

```cxx
/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct HasMember : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMember<T, std::void_t<decltype(T::m_i)>> : std::true_type { };
/**
 *  如果不包含 m_i 成员，那么 decltype(T::m_i) 就有错误，
 * 然后又根据 SFINAE 特性，编译器就不会选择特化版本，而是选择泛化版本
 */
```

暂时理解为：编译器会先看 特化版本，再去看 泛化版本
（下面再讲）

### 判断类中是否存在 某个成员函数

```cxx
/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct HasMemFunc : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>> : std::true_type { };
/**
 * 语义上，理解为是创建了一个 临时对象
 */
```

## 2 - 泛化版本 和 特化版本 到底编译器如何选择

为什么上面是 非`std::voit_t`不可？如果我将默认的 模板参数改为 `typename U = int`，会怎么样？

```cxx
/* 泛化版本 */
// template <typename T, typename U = std::void_t<>>
template <typename T, typename U = int>
struct HasMemFunc : std::false_type { };

/* 特化版本 */
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>> : std::true_type { };

int main(void)
{
    cout << HasMemFunc<NoInnerType>::value << endl; // 0 ---(int)---> 0
    cout << HasMemFunc<HaveInnerType>::value << endl; // 1 ---(int)---> 0
}
```

编译器通过一种复杂的排序规则来决定：使用类模板的泛化版本还是特化版本。

上面，特化版本虽然合适（当然编译器并不一定这么认为），但是编译器依然是原则了泛化版本。
（void 与 int 的竞争）

一般来说：void 这个干儿子（细说），跟其他类型相比，更不受待见

评价是：多写测试代码

## 3 - 借助 void_t 和 declval 实现 is_copy_assignable

std::is_copy_assignable 是 C++ 标准库的类模板，用来判断一个类对象是否可以拷贝赋值

llvm - clang 中的 is_copy_assignable 的实现：

（实现算是简单的，但是写标准库的人比较搞）

```cxx
template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_copy_assignable
    : public integral_constant<
          bool,
          __is_assignable(__add_lvalue_reference_t<_Tp>,
                          __add_lvalue_reference_t<typename add_const<_Tp>::type>)> {};
```

### 自己实现 is_copy_assignable

```cxx
class ACPABL { };

class BCPABL {
public:
    BCPABL& operator=(const BCPABL& tmpObj)
    {
        return *this;
    }
};

class CCPABL {
public:
    /* 标记为 delete 或者是 私有，那么就不能被拷贝 */
    BCPABL& operator=(const CCPABL& tmpObj) = delete;
};

/* 泛化版本 */
template <typename T, typename U = std::void_t<>>
struct IsCopyAssignable : std::false_type { };

/* 特化版本 */
template <typename T>
// struct IsCopyAssignable<T, std::void_t</* 关键是这里面怎么填写 */>>
struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&>())>>
    : std::true_type { };
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T>() = std::declval<const T&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&&>() = std::declval<const T&>()
/* std::declval<T&>() 假想创建了一个 T 的左值引用对象 */

/**
 * struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T>() = std::declval<const T&>()
 * 如果是 T 而不是 T& ，那么下面的输出结果是 0 1 1 0
 */

/**
 * struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&&>() = std::declval<const T&>()
 * 如果是 T&& 而不是 T& ，那么下面的输出结果是 0 1 1 0
 */

// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T>()
// struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&&>()
// 上面这三种，似乎没什么影响，都是 1 1 1 0

// 类对象本身都是左值，那么 std::declval<T&>() = std::declval<const T&>() 是允许的
/* std::declcal<T>() -> T&& ，那么就会 */
/* std::declcal<T&>() -> T& && （引用折叠） <==> T& */
/* std::declcal<T&&>() -> T&& && （引用折叠） <==> T&& ，右值引用 */
/* std::declval<const T&> 返回的是 const T& && <==> const T& */
/* 右值引用 = 左值 */

int main(void)
{
    BCPABL bObj1;
    BCPABL bObj2;

    cout << IsCopyAssignable<int>::value << endl; // 1
    cout << IsCopyAssignable<ACPABL>::value << endl; // 1
    cout << IsCopyAssignable<BCPABL>::value << endl; // 1
    cout << IsCopyAssignable<CCPABL>::value << endl; // 0
}
```

为什么上面要是`std::declval<const T&>`呢？就是`=`也就是`a.operator=(const T&)`，
这个与拷贝赋值运算符的形参是一致的。

## 4 - 综合范例

假设有两个容器：vector 容器，但是这两个容器中的元素类型不同。

需求：重载一下 + 运算符，做一下两个容器的加法运算。（容器的加法运算：对应相加，相同维度）
但是有一个问题：比方说 int + double，理论上应该返回是：double 类型。

考虑设计：设计一个类模板 VecAddResult，可以得到相加结果的类型

```cxx
template <typename T, typename U>
struct VecAddResult {
    using type = decltype(T() + U()); /* 把结果类型的推导，交给了编译器 */
};

template <typename T, typename U>
std::vector<typename VecAddResult<T, U>::type> operator+(std::vector<T> const& vec1, std::vector<U> const& vec2)
{
    std::vector<typename VecAddResult<T, U>::type> tmpVec;
    return tmpVec;
}

int main(void)
{
    auto m = int() + double();
    cout << typeid(m).name() << endl;

    std::vector<int> veca { 0, 10, 20 };
    std::vector<double> vecb { 1.1, 2.2, 3.3 };
    auto vecc = veca + vecb;

    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(vecc)>().pretty_name() << endl;
    // std::__1::vector<double, std::__1::allocator<double>>
}
```

如果是自定义类型，使用默认的构造函数

```cxx
struct elemC {
    elemC operator+(const elemC& tmppar);
};

int main(void)
{
    vector<elemC> veca;
    vector<elemC> vecb;
    auto vecc = veca + vecb;
    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(vecc)>().pretty_name() << endl;
    // std::__1::vector<elemC, std::__1::allocator<elemC>>
}
```

但是如果我们在 elemC 中手动实现了一个 有参构造，会抑制编译器合成出来 默认构造，
那么`decltype( T() + U() )`会有问题的

### T() + U() 的问题

```cxx
struct elemC {
    elemC(int tmpValue) { }
    elemC operator+(const elemC& tmppar);
};

int main(void)
{
    vector<elemC> veca;
    vector<elemC> vecb;
    auto vecc = veca + vecb; /* 错误： Invalid operands to binary expression ('vector<elemC>' and 'vector<elemC>') */
    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(vecc)>().pretty_name() << endl;
    // std::__1::vector<elemC, std::__1::allocator<elemC>>
}
```

所以还是不能通过 `T()` 这种方式创建临时对象。
那还有什么方式可以绕过 构造函数创建临时对象呢?（书接上回，chap4_1） ---> 使用`std::declval`

改一下 VecAddResult：

```cxx
template <typename T, typename U>
struct VecAddResult {
    // using type = decltype(T() + U()); /* 把结果类型的推导，交给了编译器 */
    using type = decltype(std::declval<T>() + std::declval<U>()); /* 把结果类型的推导，交给了编译器 */
};

/* 写一个 别名，不用那么啰嗦 */
template <typename T, typename U>
using VecAddResult_t = typename VecAddResult<T, U>::type;

template <typename T, typename U>
std::vector<VecAddResult_t<T, U>> operator+(std::vector<T> const& vec1, std::vector<U> const& vec2)
{
    std::vector<VecAddResult_t<T, U>> tmpVec;

    return tmpVec;
}
```

但是现在还有一个问题，就是：两个东西不一定是可以 相加的（如果没有重载的运算符的话）

### 利用 SFINAE 特性，省略不可相加的情况

如果不能相加的话，那么就是直接报错：

```cxx
error: p3.cxx:11:45: error: invalid operands to binary expression ('elemC' and 'elemC')
   11 |     using type = decltype(std::declval<T>() + std::declval<U>()); /* 把结果类型的推导，交给了编译器 */
      |                           ~~~~~~~~~~~~~~~~~ ^ ~~~~~~~~~~~~~~~~~
```

这种报错有些不地道。我们希望：模板能够被实例化，就能够被正确的实例化，而不是编译的时期才出现 编译错误

```cxx
/* 泛化版本 */
template <typename, typename, typename = std::void_t<>> /* T, U, V 在实现体中用不到 */
// template <typename T, typename U, typename V = std::void_t<>>
struct IfCanAdd : std::false_type { };

/* 特化版本 */
template <typename T, typename U>
struct IfCanAdd<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type { };

/* 泛化版本 */
template <typename T, typename U, bool ifcnado = IfCanAdd<T, U>::value>
struct VecAddResult {
    using type = decltype(std::declval<T>() + std::declval<U>());
};

/* 特化版本 */
template <typename T, typename U>
struct VecAddResult<T, U, false> { };
```

VecAddResult 和 IfCanAdd 这两个类模板之间有一种 SFINAE-friendly（SFINAE 友好关系）

SFINAE 友好关系就是：你(IfCanAdd)有 true_type，我(VecAddResult)就有 type；你没有 true_type，我就没有 type。

什么特化？什么泛化？
上面 VecAddResult 的泛化版本，里面有了`using type = decltype(std::declval<T>() + std::declval<U>());`，
就代表是 true_type 友，那么特化版本就是 false_type 友

这个时候，如果没有 operator+，那么就会有下面的报错：

```cxx
error: p4.cxx:77:22: error: invalid operands to binary expression ('vector<elemC>' and 'vector<elemC>')
   77 |     auto vecc = veca + vecb;
      |                 ~~~~ ^ ~~~~
```

之前的报错，不明不白（如果没学过 模板编程的话）；
现在这个报错就清楚多了，是直接在这里报错，而不是在模板的部分报错。

### 模板编程原则

如果选择要实例化某个模板（operator+、VecAddResult）， 则实例化时不应该失败（不应该在实例化出现编译错误）
