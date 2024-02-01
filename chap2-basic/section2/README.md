# chap2_2 - 类模板、变量模板、别名模板

## 1 - 类模板的 基本范例 和 模板参数 的推断

### 基本范例

类模板：是产生类的模具，通过给定的模板参数，生成具体的类，
也就是实例化一个特定的类。`vector<int>`

类模板的声明与实现 要放到 同一个文件之中。

```cxx
/**
 * @brief 类模板定义
 *
 * @tparam T
 */
template <typename T>
class MyVector {
public:
    typedef T* myIterator;

public:
    MyVector();
    MyVector& operator=(const MyVector&); // 赋值运算符重载

public:
    void myfunc()
    {
        cout << "myfunc()被调用" << endl;
    }

public:
    myIterator mybegin();
    myIterator myend();
};
```

单纯的`MyVector`这个只是一个类模板（类名），但是如果是`MyVector<int>`就是一个具体的类型了（类型名）。

```cxx
template<typename T>
class MyVector {
    // MyVector<int>& operator=(const MyVector&); // 在类定义中，可以被省略
    MyVector& operator=(const MyVector&); // 赋值运算符重载
};
```

在类定义里面中，类型名 可以简化为 类名

注意：类模板是没法省略：模板参数的（但是 C++17 以后，这点优化了）

类模板中，只有被调用的成员函数（包括静态成员函数），编译器才会产生出这些函数的实例化代码。

不同的模板参数，对应的 静态数据成员 是不一样的。

### 模板参数的推断

C++17 以后，类模板的类型模板参数也可以推断了。

```cxx
class MyVec{
	MyVector(T v);
};

int main(void) {
    MyVector tmpvec2(12);
}
```

这也是可以推断的

### 推断指南(deduction guide)概要了解

C++17 新概念：主要用来在推断 类模板参数 时，提供 推断指南

#### 隐式的推断指南

```cxx
template <typename T>
struct A {
    A(T val1, T val2)
    {
        cout << "A::A(T val1, T val2)执行了" << endl;
    }

    A(T val)
    {
        cout << "A::A(T val)执行了" << endl;
    }
};

int main(void)
{
    A aObj1(15, 16); /* int */

    A aOjb2(12.8); /* double */
}
```

针对模板 A 的每个构造函数，都有一个隐式的模板参数推断机制存在，
这个机制又叫做隐式推断指南

这个指南长下面这个样子：

```cxx
template<typename T>
A(T, T) -> A<T>;  // 含义：出现左侧这种形式的时候，推断成右侧这种类型。右侧类型也被称为 “指南类型”
```

例如，我在 insight++ 里面看到的是这样的：

```cxx
/* First instantiated from: s2_2.cxx:20 */
#ifdef INSIGHTS_USE_TEMPLATE
template <>
A(int val1, int val2) -> A<int>;
#endif
```

- -> 左侧部分：该推断所对应的构造函数的函数声明，多个参数之间用`,`分隔
- -> 右侧部分：类模板明，接着一个尖括号，尖括号中是模板参数名
- 整个推断指南的含义：当用调用带两个参数的构造函数通过类模板 A 创建相关的对象时，
  请用所提供的构造函数的实参来推断类模板 A 的模板参数

一句话：推断指南的存在意义，就是让编译器：能够把模板参数的类型推断出来。

如果有这样的推断指南：

```cxx
template<typename T>
A(T, T) -> A<double>;
```

那么

`A aObj1(15, 16)`相当于是`A<double> aObj1(15, 16)`

是这样的：

```cxx
template <typename T>
struct A {
    T m_b;
};
template <typename T>
A(T) -> A<double>;

int main(void)
{
    A<int> aObj1; /* 需要明确指定模板参数类型 */

    A<int> aObj2 { 15 }; /* 需要明确的指定 模板参数类型 */
    A aObj3 { 15 };
    cout << aObj3.m_b << endl;
    cout << typeid(aObj3.m_b).name() << endl; // d
}
```

#### 自定义的推断指南

```cxx
template <typename T>
struct A {
    T m_b;
};

int main(void)
{
    A<int> aObj1; /* 需要明确指定模板参数类型 */

    A<int> aObj2 { 15 }; /* 需要明确的指定 模板参数类型 */
    A aObj3 { 15 }; /* 错误：no viable constructor or deduction guide for deduction of template arguments of 'A' */
}
```

报错：没有 可访问的构造函数（其实构造函数的深层原因是：每个构造函数都隐含一个推断指南，归根解底还是要推断指南）、推断指南。
报错的深层原因就是：没有办法推断出 m_b 的类型，
也就没有办法分配内存。

我们可以不写构造函数，但是写一个 推断指南

```cxx
template <typename T>
struct A {
    T m_b;
};
template <typename T>
A(T) -> A<T>;

int main(void)
{
    A<int> aObj1; /* 需要明确指定模板参数类型 */

    A<int> aObj2 { 15 }; /* 需要明确的指定 模板参数类型 */
    A aObj3 { 15 };
}
```

不再报错的原因：

1. 类 B 是聚合类，是可以通过 `{}`初始化的
2. `B bobj3 { 15 }`这种形式正好就相当于调用了一个 类模板 B 的一个带参数（参数为 15）的构造函数，尽管 类模板 B 中 实际上并不存在构造函数。
3. 因为`template<typename T> B(T) -> B<T>`推断指南存在，当调用了类模板 B 带一个参数的构造函数时，推断出来的类型为`B<T>`，
   所以最终推断出来的类型为`B<int>`类型

## 2 - 类模板的各种特化

一般来讲：所写的类模板都是泛化的类模板

特化的类模板是通过泛化的类模板来生成的，所以：得先有泛化版本，才能有特化版本。

所谓特化版本，就是特殊对待的版本。

全特化：就是把 TC 这个泛化版本的所有模板参数都用具体的类型来代替，构成一个特殊的版本（全特化版本）

### 类模板的全特化

```cxx
template <typename T, typename U>
struct TC {
    TC()
    {
        cout << "TC 泛化版本 构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }
};

template <>
struct TC<int, int> {
    TC()
    {
        cout << "TC<int, int> 特化版本 构造函数" << endl;
    }
};

int main(void)
{
	mytc2.functest1(); /* 错误： No member named 'functest1' in 'TC<int, int>' */
}
```

就是我们全特化以后，就是一个全新的类了，那么也就要在新类型中 定义函数 才能正常使用。
（尽管这里是全特化，但是更像是一个普通类、全新的类了）

在理解上：泛化版本的类模板 与 全特化版本的类模板，这是名字相同（都叫 TC），
在其他方面，可以把实例化后的他们理解成是两个完全不同的类

全特化版本的类外定义：

```cxx
template <>
struct TC<int, int> {
    TC()
    {
        cout << "TC<int, int> 特化版本 构造函数" << endl;
    }

    void functest1();
};

void TC<int, int>::functest1()
{
    cout << "TC<int, int>::functest1" << endl;
}
```

### 普通成员函数的全特化

```cxx
template <typename T, typename U>
struct TC {
    TC()
    {
        cout << "TC 泛化版本 构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }
};

template <>
void TC<double, int>::functest1()
{
    cout << "普通成员函数 TC<double, int>::functest1 的全特化版本" << endl;
}

int main(void)
{
    TC<double, int> a;
    a.functest1(); // 普通成员函数 TC<double, int>::functest1 的全特化版本
}
```

### 静态成员函数的全特化

```cxx
template <typename T, typename U>
struct TC {
    TC()
    {
        cout << "TC 泛化版本 构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }

    static int m_stc; /* 声明一个静态成员变量 */
};

template <typename T, typename U>
int TC<T, U>::m_stc = 50; /* 定义一个静态成员变量 */

template <> /* 静态成员变量的全特化 */
int TC<int, int>::m_stc = 100;

int main(void)
{
    TC<double, float> df;
    cout << df.m_stc << endl; // 50

    TC<int, int> ii;
    cout << ii.m_stc << endl; // 100
}
```

如果进行了：普通成员函数的全特化，或者是静态成员变量的全特化，
那么就无法用这些全特化时指定的类型 来对 整个类模板进行全特化

```cxx
template <> /* 静态成员变量的全特化 */
int TC<int, int>::m_stc = 100;

template <>
struct TC<int, int> { }; /* 错误：Explicit specialization of 'TC<int, int>' after instantiation */
```

上面这个错误的原因：普通成员函数全特化以后，会隐式的实例化这个类`instantiation`。
然而我们这里又重新定义（特化`specialization`）了一个类

下面这个错误的原因：
我们相当于是新创建了一个类，但是这个类中不包含`m_stc`，
所以下面就没找到

```cxx
template <>
struct TC<int, int> { };

template <> /* 错误：Extraneous 'template<>' in declaration of variable 'm_stc */
int TC<int, int>::m_stc = 100;  /* 错误：No member named 'm_stc' in 'TC<int, int>' */
```

这个改一下就行了

```cxx
template <>
struct TC<int, int> {
    static int m_stc;
};

int TC<int, int>::m_stc = 100;
```

### 类模板的偏特化

#### 模板参数 数量上 的偏特化

```cxx
template <typename T, typename U>
struct TC {
    TC()
    {
        cout << "TC 泛化版本 构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }

    static int m_stc; /* 声明一个静态成员变量 */
};

template <typename U>
struct TC<float, U> {
    TC()
    {
        cout << "TC<float, U> 偏特化版本构造函数" << endl;
    }

    void functest();
};

template <typename U>
void TC<float, U>::functest()
{
    cout << "TC<float, U>::functest() 偏特化版本" << endl;
}

int main(void)
{
    TC<float, int> myTc4;
    myTc4.functest();

    return 0;
}
```

#### 模板参数 范围上 的偏特化

下面例子从：T ---> const T，U ---> U\*

```cxx
template <typename T, typename U>
struct TC<const T, U*> {
    TC()
    {
        cout << "TC<const T, U*> 偏特化版本构造函数" << endl;
    }

    void functest();
};

template <typename T, typename U>
void TC<const T, U*>::functest()
{
    cout << "TC<const T, U*>::functest 偏特化版本" << endl;
}

int main(void)
{
    TC<const int, int*> t; // TC<const T, U*> 偏特化版本构造函数
    t.functest(); // TC<const T, U*>::functest 偏特化版本
    return 0;
}
```

## 3 - 缺省参数

### 常规缺省参数

类型模板参数缺省值规矩：如果某个模板参数有缺省值，那么从这个有缺省值的模板参数开始，后面的所有模板参数都要有缺省值

```cxx
template <typename T = int, typename U> /* 错误：U: Template parameter missing a default argument */
struct TC {
    TC()
    {
        cout << "TC 泛化版本的构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }
};

int main(void)
{
    TC<> mytc5; /* <default, default> */
    TC<double> mytc6; /* <double, default> */
    TC<double, double> mytc7; /* <double, double> */
    return 0;
}
```

类模板偏特化版本中的类型模板参数不可以有缺省值

```cxx
/**
 * @brief 偏特化版本（不能有缺省值！）
 *
 * @tparam T
 */
template <typename T = char> /* 错误： Default template argument in a class template partial specialization */
struct TC<T, int> { };
```

如果特化版本中没有缺省值，我们知道：特化只是泛化的子集。编译器会在泛化的版本中推断的

### 后面的模板参数以前的模板参数

```cxx
template <typename T, typename U = T*>
struct TC {
    /* ... */
};

int main(void)
{
    TC<double> mytc5; /* <double, double*> */
    TC<double, int> mytc6; /* <double, int> */
}
```

### 在模板声明中指定缺省参数

不建议采用这种语法，这种语法非常不常用。

```cxx
// 声明 1
template <typename T, typename U, typename V = int, typename W = char>
struct TC;

// 声明 2 （错误写法）
/* 错误： V W : Template parameter redefines default argument */
template <typename T, typename U = double, typename V = int, typename W = char>
struct TC;
```

下面这种写法是正确的

```cxx
// 声明 1
template <typename T, typename U, typename V = int, typename W = char>
struct TC;

// 声明 2
template <typename T, typename U = double, typename V, typename W>
struct TC;

// 定义
template <typename T, typename U, typename V, typename W>
struct TC { };

int main(void)
{
    TC<char> mytc; /* <char, default, default, default> */
}
```

## 4 - 类型别名

考虑到类型名比较长，所以一般用 typedef，或者 using 给这些类型名 起一个额外的别名 来简化书写

```cxx
typedef TC<int> TF_TC;
using U_TC = TC<double>; // 这种编程语法比较好

int main(void)
{
    TF_TC myTC1;
    U_TC myTC2;
}
```

推荐使用 using（C++11 以后）

## 5 - 非类型模板参数

```cxx
template <typename T, typename U, auto arrsize = 8>
struct TC {
    T m_arr[arrsize]; /* 因为 arrsize 是：编译期间就确定的 */
    void functest();
};

template <typename T, typename U, auto arrsize>
void TC<T, U, arrsize>::functest()
{
    cout << "functest 泛化版本" << endl;
}

int main(void)
{
    TC<double, double> mytc1;
    for (size_t i = 0; i < 8; ++i) {
        mytc1.m_arr[i] = static_cast<double>(i);
    }
    cout << mytc1.m_arr[7] << endl; // 7

    TC<double, double, 18> mytc2;
    mytc2.m_arr[10] = 16.8 / 0.0; // inf
    cout << mytc2.m_arr[10] << endl;
}
```

非类型模板参数，数值是常量，类型一般也限定在整型，类型可以是`auto`

### 全局指针不能作为函数参数

```cxx
template <const char* p>
struct TC {
    TC()
    {
        printf("TC::TC 执行了, p= %s\n", p);
    }
};

const char* g_s = "hello"; /* 全局指针 */

int main(void)
{
    TC<g_s> myTC; /* 错误： Non-type template argument is not a constant expression */
    return 0;
}
```

但是 数组 居然可以！
（感觉这个是编译器直接不允许这种）

数组是指针常量。因为指针有被修改成指向其他的可能。

```cxx
template <const char* p>
struct TC {
    TC()
    {
        printf("TC::TC 执行了, p= %s\n", p);
    }
};

const char g_s[] = "hello"; /* 常量数组 */
int main(void)
{
    TC<g_s> myTC;
    return 0;
}
```

### 字符串常量也是无法作为 模板参数

```cxx
int main(void) {
	TC<"hello"> myTc2; /* 错误：Pointer to subobject of string literal is not allowed in a template argument */
}
```

就是如果 字面量相同，都是 "hello"。

字符串常量，有的可能是存储在 “字面量池” 中；有的可能是存储在 “全局常量区“ 中。
两个 字面上 是一样的，但是 内存地址可能不一样。

同理：为什么也不允许 float、double 作为 非类型模板参数 存在呢？
因为 浮点数保存的并不是一个精确的数字。
比方说，我们会`f1 - f2 <= 10e-6`来判断相等。

## 6 - 成员函数模板

### 基本概念、构造函数模板

```cxx
template <typename T1>
class A {
public:
    template <typename T2>
    A(T2 v1, T2 v2); /* 构造函数模板，引入了自己的模板参数 T2 */

    template <typename T3> /* 普通成员函数 */
    void myft(T3 tmpt)
    {
        cout << tmpt << endl;
    }

    T1 m_ic;
    static constexpr int m_stcvluae = 200;
};

/* 类外实现类模板的构造函数 */
template <typename T1>  // 这两个 template 不能交换顺序
template <typename T2>
A<T1>::A(T2 v1, T2 v2)
{
    cout << "A<T1>::A(T2 v1, T2 v2) 执行了!" << endl;
}

int main(void)
{
    A<float> a(1, 2); /* 函数是可以推断的 */
}
```

上面这个：实例化出来了`A<float>`这样一个类型，并用 int 来实例化构造函数

一些说法

1. 类模板中的成员函数，只有源程序代码中出现调用这些成员函数代码时，这些成员函数才会出现在一个 实例化了的 类模板中。

如果没有`A<float> a2(1.1, 2.2);`，编译器也就不会实例化出来`A<float>::A(double v1, double v2)`

2. 类模板中的成员函数模板，只有源程序代码中出现 这些成员函数模板的 来嘛时，这些成员函数模板的具体实例才会 出现在一个实例化了的类模板中。

3. 目前编译器并不支持 虚成员函数模板

因为，虚函数表 vtbl 的大小的是固定的。
成员函数模板只有被调用的时候才能被实例化出来，

如果允许虚函数模板，则每次有人用新的参数类型调用该函虚函数模板时，
就必须啊给对应的虚函数表再增加一项，这意味着：只有链接程序才能去构造虚函数表并在表中设置有关函数，
因此，成员函数模板绝不能是虚的。

但是，类模板中可以有普通的虚成员函数，这并没有什么问题。
因为，普通成员函数如果不被调用的情况下不会被实例化出来。
但是，对于虚函数，不管是否被调用，编译器都会把他实例化出来，
因为编译器要创建 虚函数表 vtbl，该表中的具体表项都对应一个虚函数地址，
所以编译器必然得把所有虚函数都实例化出来。

#### 类模板的 构造函数 与 构造函数模板

```cxx
template <typename T1>
class A {
public:
    template <typename T2>
    A(T2 v1, T2 v2); /* 构造函数模板，引入了自己的模板参数 T2 */

    A(double v1, double v2)
    {
        cout << "A<T1>::A(double, double) 执行了" << endl;
    }

    A(T1 v1, T1 v2) /* 不是构造函数模板，而是构造函数 */
    {
        cout << "A<T1>::A(T1, T1) 执行了" << endl;
    }
};

int main(void)
{
    A<float> a2(1.1, 2.2); /* A::A(double, double) */
    A<float> a3(1.1f, 2.2f); /* A::A(T1, T1) */
}
```

### 拷贝构造函数模板 与 拷贝赋值运算符模板

```cxx
template <typename T1>
class A {
public:
    /* 拷贝构造函数模板 */
    template <typename U>
    A(const A<U>& other)
    {
        cout << "A::A(const A<U>& other) 拷贝构造函数模板执行了!" << endl;
    }

    /* 拷贝构造赋值 模板 */
    template <typename U>
    A<T1>& operator=(const A<U>& other)
    {
        cout << "A::operator=(const A<U>& other) 拷贝赋值运算符模板执行了 !" << endl;
        return *this;
    }
};
```

注意：拷贝构造函数模板时不是 拷贝构造函数；拷贝赋值运算符模板 不是 拷贝赋值运算符函数（模板不是函数）

因为拷贝构造函数或者拷贝赋值运算符，要求拷贝的对象类型完全相同，
而 拷贝构造函数模板 或者 拷贝赋值运算符，要求拷贝的对象类型并不一样相同`template <typename U> A(const A<U>& other)`

```cxx
int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.m_ic = 3.14;
    A<float> a4(a3); /* 没有执行 拷贝构造模板代码 */
    cout << a4.m_ic << endl; // 3.14
}
```

执行上面这一段代码，发现两个问题：

1. `A<float> a4(a3);` 代码行没有输出任何结果，这表示拷贝构造函数模板中的代码没有执行
2. `a4.m_ic`的值确实变成了 16.2。这说明确实是通过 a3 拷贝构造生成的 a4
   a3、a4 类型相同`A<float>`，本该执行拷贝构造函数，但是因为类模板 A 中没有拷贝构造函数，
   所以编译器内部实际是执行了 bitwise 的拷贝动作，使 a4.m_ic 值变成了 16.2 了。

但是为什么不调用 拷贝构造函数模板呢？（这个拷贝构造模板，这个就像是一个 有参构造）
固定搭配（死记硬背）秒了：拷贝构造函数模板 永远不可能称为 拷贝构造函数。
编译器不会调用 拷贝构造函数模板，来代替 拷贝构造函数。

那么什么时候调用 拷贝构造函数模板呢？类型不同的时候就会调用，比方说`A<float> a5( A<double>() )`

但是很曹丹的就是：
（非常不建议这么写，因为 拷贝构造函数都是有 const 的）

```cxx
template <typename T1>
class A {
public:
    T1 m_ic;

public:
    // /* 拷贝构造函数模板 */
    // template <typename U>
    // A(const A<U>& other)
    // {
    //     cout << "A::A(const A<U>& other) 拷贝构造函数模板执行了!" << endl;
    // }

    /* 拷贝构造函数模板 */
    template <typename U>
    A(A<U>& other)
    {
        cout << "A::A(const A<U>& other) 拷贝构造函数模板执行了!" << endl;
    }
};

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.m_ic = 3.14;
    A<float> a4(a3); /* A::A(const A<U>& other) 拷贝构造函数模板执行了! */
    cout << a4.m_ic << endl; // (乱码，未初始化)
}
```

**拷贝赋值运算符模板 与 拷贝构造函数模板 的行为是相同的**

下面是真正的拷贝构造函数：

```cxx
class A{
	/* 真正的 拷贝构造函数 */
    A(const A& other)
    {
        cout << "A<T1>::A(const A<T1>& other) 拷贝构造函数执行了" << endl;
    }
};

int main(void) {
    A<float> a3(11.1f, 12.2f);
    a3.m_ic = 3.14;
    A<float> a4(a3); /* A<T1>::A(const A<T1>& other) 拷贝构造函数执行了 */
    cout << a4.m_ic << endl;
}
```

#### const ? 非 const ? （该有 const 就要有 const）

```cxx
template <typename T1>
class A {
    T1 m_ic;

public:
    /* 拷贝构造函数模板 */
    template <typename U>
    A(A<U>& other)
    {
        cout << "A::A(const A<U>& other) 拷贝构造函数模板执行了!" << endl;
    }

    /* 真正的 拷贝构造函数 */
    A(const A& other)
    {
        cout << "A<T1>::A(const A<T1>& other) 拷贝构造函数执行了" << endl;
    }
};

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.m_ic = 3.14;
    A<float> a4(a3); /* A::A(const A<U>& other) 拷贝构造函数模板执行了! */
    cout << a4.m_ic << endl;
}
```

### 特化

#### 类内实现

```cxx
class A{
public:
    template <typename T3, typename T4>
    void myft(T3 tmpt1, T4 tmpt2)
    {
        cout << "myft(T3, T4) 泛化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }

    template <typename T4>
    void myft(int tmpt1, T4 tmpt2)
    {
        cout << "myft(int, T4) 偏特化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }

    template <>
    void myft(int tmpt1, float tmpt2)
    {
        cout << "myft(int, float) 全特化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }
};

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.myft(3, 2.5f); // myft(int, float) 全特化版本
    a3.myft(3, 3.5); // myft(int, T4) 偏特化版本
}
```

#### 类外实现 偏特化

```cxx
template <typename T1>
template <typename T4>
void A<T1>::myft(int tmpt1, T4 tmpt2)
{
    cout << "myft(int, T4) 偏特化版本" << endl;
    cout << tmpt1 << endl;
    cout << tmpt2 << endl;
}
```

#### 类外实现 全特化

```cxx
template <typename T1>
/* 错误： Out-of-line definition of 'myft' does not match any declaration in 'A<T1>' */
void A<T1>::myft(int tmpt1, float tmpt2)
{
    cout << "myft(int, float) 全特化版本" << endl;
    cout << tmpt1 << endl;
    cout << tmpt2 << endl;
}
```

有些资料显示：目前 C++ 标准不允许在类模板之外 全特化 一个未被特化的类模板的成员函数

整体感觉：类模板中的成员函数全特化可能还不算太完善，写代码时要注意测试。
在实际上工作中：尽量把这些特化版本写在类模板内部，然后类模板也要写在头文件中

```cxx
template <>
class A<float> {
public:
    A(float v1, float v2) /* 不是构造函数模板，而是构造函数 */
    {
        cout << "A<T1>::A(T1, T1) 执行了" << endl;
    }

    template <typename T3, typename T4>
    void myft(T3 tmpt1, T4 tmpt2)
    {
        cout << "A<char*>::myft(int, float) 泛化版本" << endl;
        cout << tmpt1 << endl;
        cout << tmpt2 << endl;
    }

    /* 可声明可不声明 */
    // template <>
    // void myft(int tmpt1, float tmpt2);
};

/* A<float>中有泛化版本的 myft，因此不用在 A<float> 中声明如下的全特化版本了 */
template <>
void A<float>::myft(int tmpt1, float tmpt2)
{
    cout << "myft(int, float) 全特化版本" << endl;
    cout << tmpt1 << endl;
    cout << tmpt2 << endl;
}

int main(void)
{
    A<float> a3(11.1f, 12.2f);
    a3.myft(3, 3.5f); // myft(int, float) 全特化版本
}
```

## 7 - 类/类模板 中的 类模板（嵌套）

```cxx
template <>
class A<float> {
public:
    template <typename U>
    class OtherC {
    public:
        void myfOC()
        {
            cout << "myfOC 执行了" << endl;
        }
    };
};
```

其实就看成两个毫无关系的类就行了。

## 8 - 变量模板 与 成员变量模板

变量模板：variable templates（C++14 以后引入）（放到全局空间中）

```cxx
template <typename T>
T g_myVar {}; /* 初始化 */

int main(void)
{
    g_myVar<float> = 15.6f;
    g_myVar<int> = 13;

    cout << g_myVar<float> << endl; // 15.6
    cout << g_myVar<int> << endl; // 13
}
```

变量模板，从感觉上，变量模板与函数模板有些相似，看起来像一个没有参数，
但是有返回值的函数模板

#### 零初始化

1. `int temp1 = int();`

2. `int tempt2 = {};`

3. `int temp3 = int{};`

pointer ---> nullptr；int ---> 0；bool ---> false。

### 变量模板的特化

#### 全特化

```cxx
template <typename T>
T g_myVar {}; /* 初始化 */

template <>
char g_myVar<double> {};

int main(void)
{
    cout << typeid(g_myVar<double>).name() << endl; // c
}
```

#### 偏特化

```cxx
template <typename T>
T g_myVar<T*> { 120 }; /* 要求 T* 必须依赖于 T */

int main(void)
{
    cout << typeid(g_myVar<int*>).name() << endl;
    cout << g_myVar<int*> << endl;
}
```

上面这个看起来是： 范围上的偏特化

### 默认模板参数

```cxx
template <typename T = int>
T g_myVar {}; /* 初始化为0 */

int main(void)
{
    g_myVar<> = 26;
    cout << g_myVar<> << endl;
    // cout << g_myVar << endl; /* 错误： Use of variable template 'g_myVar' requires template arguments */
}
```

不能忽略`<>`尖括号

### 非类型模板参数

```cxx
template <typename T = int, int arrsize = 20>
T g_myVar[arrsize];

int main(void)
{
    for (int i = 0; i < 15; i++) {
        g_myVar<int, 15>[i] = i;
    }

    for (int i = 0; i < 15; i++) {
        cout << g_myVar<int, 15>[i] << endl;
    }
    return 0;
}
```

### 变量模板的另一种形式

```cxx
template <typename T>
struct B {
    const static T value = { 160 };
    // constexpr static T value = { 160 }; 也是可以的
};

template <typename T>
int g_myVar = B<T>::value;

int main(void)
{
    cout << g_myVar<int> << endl; // 160
    g_myVar<int> = 152;
    cout << g_myVar<int> << endl; // 152
    cout << B<int>::value << endl; // 160
    return 0;
}
```

注意这种写法

### 成员变量模板

```cxx
template <typename T>
class D {
public:
    template <typename W>
    static W m_tpi; // 静态成员变量模板声明
};

// 静态成员类外初始化，别忘了
template <typename T>
template <typename W>
W D<T>::m_tpi = {};

int main(void)
{
    cout << D<float>::m_tpi<int> << endl; // 5
    D<float>::m_tpi<int> = 150;
    cout << D<float>::m_tpi<int> << endl; // 150
    cout << D<int>::m_tpi<int> << endl; // 5
    cout << D<int>::m_tpi<float> << endl; // 5
}
```

## 9 - 别名模板 与 成员别名模板

别名模板：alias templates（C++11 以后）
引入的目的：不但可以简化书写，而且可以达到一些通过其他手段难以达到的效果

```cxx
template <typename T>
using str_map_t = map<string, T>; /* 别名模板 */

int main(void)
{
    str_map_t<int> m; // 相当于是 map<string, int>
    m.insert({ "first", 1 });
    m.insert({ "second", 2 });

    for (auto v : m) {
        cout << v.first << " " << v.second << endl;
    }
}
```

#### 成员别名模板

```cxx
template <typename T>
class E {
    template <typename U>
    using str_map_t = map<string, U>;

public:
    str_map_t<int> m;
};

int main(void)
{
    E<float> e;
    e.m.insert({ "first", 1 });
    e.m.insert({ "second", 2 });
    for (auto v : e.m) {
        cout << v << endl;
    }
}
```

## 10 - 模板模板参数

模板模板参数：template template parameters，让模板参数本身称为模板的意思

1. int：简单类型、内部类型
2. vector，list：是 C++ 标准库中的容器，类模板（类名），`vector<int>` 或者 `list<double>`就属于模板被实例化了的参数，称之为 类型（类类型）

需求：创建 myclass 的类模板，成员变量 myc，是一个容器。
比方说`myclass <int, vector> myvector;`，其中`int`是容器中的元素类型，vector 是容器类型。

但是，下面这段代码是错误的：

```cxx
/* 错误示范 */
template <typename T, typename Container = vector>
class myclass {
public:
    Container<T> myc;
};
```

因为是不符合语意的，`typename`后面应该跟着一个 类型，而不是一个 类模板，
这里的`std::vector`是一个类模板，并不是一个类型。

Container，不叫 类型 模板参数，而叫 模板 模板参数，
表示这个模板参数本身又是一个模板。

```cxx
template <
    typename T,
    // typename Container = vector
    template <class> class Container = vector // 这就是一个模板模板参数，写法比较固定，这里的名字叫Container，叫U也可以
>
class myclass {
public:
    Container<T> myc;
};
```

这句话`template<class> class Container`就像是定义一个普通的类一样。

明白这个是什么意思了，vector 是模板，想想 vector 是如何定义的`template<typename T> class vector;`
和上面的如出一辙，只不过我们把这里的`vector`换成了一个参数`Container`

### template <typename W> typename Container

```cxx
template <
    typename T,
    // typename Container = vector
    // template <class> class Container = vector
    template <typename W> class Container = vector // 这两段话实际上是同一个意思，只不过上面是 省略了这个W
	// 这个 W 在这个例子是没有用，W 也 不能放在 myclass 中使用。但是 W 也是有用的
	// 这个 W 叫做 Container(模板模板参数) 的模板参数
    >
class myclass {
public:
    Container<T> myc;
};
```

下面这种语法，占位也是允许的

```cxx
template <
    typename T,
    template <typename W> class = vector // 省略
    >
class myclass {
public:
    // Container<T> myc; // 因为上面 Container 省略了，这里不能用了
};
```

上面这段代码还是有些问题的，gpt 给了如下代码：

```cxx
template <
    typename T,
    template <typename W, typename = std::allocator<W>> class Container = vector
>
class myclass {
public:
    myclass() {
        for (int i = 0; i < 10; i++) {
            myc.push_back(i);
        }
    }

public:
    Container<T> myc;
};
```

## 11 - 共用体模板（联合模板）

可以把联合体理解成一种 类类型。联合也支持模板化
