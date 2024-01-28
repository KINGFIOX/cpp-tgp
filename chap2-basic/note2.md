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

### 后面的模板参数以来前面的模板参数

### 在模板声明中指定缺省参数

## 4 - 类型别名

## 5 - 非类型模板参数
