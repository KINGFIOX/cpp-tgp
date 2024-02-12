# chap3_4 - std::enable_if 的使用

从这节课开始，就要逐步从：面向对象 ----> 泛型编程

## 1 - 一个简单范例

```cxx
template <typename T>
void myfunc(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc(int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

template <typename T>
void myfunc1(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc1(unsigned int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

template <typename T>
void myfunc2(const T& t)
{
    cout << "myfunc 函数模板执行了" << endl;
}

void myfunc2(unsigned short int tmpvalue)
{
    cout << "myfunc 普通函数执行了" << endl;
}

int main(void)
{

    myfunc(15); /* myfunc 普通函数执行了 */
    /* 开发者的直觉：如果 函数模板 与 普通函数 都满足，感觉上是优先使用 普通函数 */

    myfunc1(15); /* myfunc 函数模板执行了 */
    /* 15，编译器一般认为是 signed int。如果调用 普通函数，要隐式转换为 unsigned int，
    但是如果直接套用模板，就不用转换，编译器觉得 模板更加合适 */

    myfunc1(15U); /* myfunc 普通函数执行了 */

    myfunc2(15); /* 函数模板执行了 */
}
```

对于重载的函数 或者 函数模板的选择上，编译器内部有一个自己的规则，
并不是简单粗暴的对函数就有限选择，也不是函数模板就靠后选择

## 2 - 替换失败并不是错误（SFINAE, substitude failure is not a error）

可以看成是 C++的一种特性，或者一种模板设计中要遵循的原则

SFINAE 特性是针对重载而言的。

针对于函数模板而言，当用一个具体类型替换函数模板的参数时，
可能会产生一项不到的问题，比方说：产生一些毫无意义，甚至是看起来语法上有错误的代码。
对于这些代码，编译器不一定报错，有可能是忽略，因为编译器认为这个模板不匹配针对本次的函数调用，
就当这个函数模板不存在一样；转而去选择其他更匹配的函数或者 函数模板

### 例子

```cxx
template <typename T>
typename T::size_type myDouble(const T& t)
{
    return t[0] * 2;
}

int main(void)
{
    myDouble(15);
}
```

报错如下：

```cxx
error: s3_8.cxx:13:5: error: no matching function for call to 'myDouble'
   13 |     myDouble(15);
      |     ^~~~~~~~
s3_8.cxx:6:23: note: candidate template ignored: substitution failure [with T = int]: type 'int' cannot be used prior to '::' because it has no members
    6 | typename T::size_type myDouble(const T& t)
      |          ~            ^
1 error generated.
  > in s3_8.cxx
```

从这个报错可以看出，一般是直接替换了，也就是变成了`typename int::size_type myDouble(const T&)`，
这个如果正常写的话，肯定还报错无疑了。但是如果是模板的话，这个代码虽然有问题，但是编译器还是会直接忽略掉（只是一个 note）

特性：我（编译器）虽然看不出你（实例化了的模板）的对错（错误一般指无效类型，无效的表达式等），
但是我能决定是否选择你。当我觉得不合适的时候，我虽然不说你错，我会忽略你（而不会选择你）。

## 3 - enable_if

### enable_if 的基本认识

是 C++11 新标准中引入的类模板（结构模板）。使用体现了 C++编译器的 SFINAE 特性。

定位为一个 helper 模板（助手模板），用于辅助其他模板的设计，表现一种：
编译器的分支逻辑（编译器就可以确定走哪条分支）

```cxx
template <bool, class _Tp = void>
struct enable_if { }; // 泛化版本

template <class _Tp> // 偏特化版本
struct enable_if<true, _Tp> {
    using type = _Tp;
};
/* 只有这个偏特化版本存在，才存在一个名字叫做 type 的类型别名 */
```

偏特化，完全可以理解为：条件分支语句（而且是编译期的 条件分支语句）

1. 如果是 true，那么就有 type
2. 如果是 false，那么就是泛型了，啥也没有

看下面这段代码：

```cxx
    std::enable_if<(3 > 2)>::type* pointer1 = nullptr; // 没有第二个模板参数，那么默认是 void（看上面）
```

但是下面这段代码是错误的

```cxx
    std::enable_if<(3 < 2)>::type* pointer1 = nullptr;  // 因为没有 type 这个成员
```

报错如下：

```cxx
error: s3_9.cxx:17:5: error: no template named 'type' in 'std::enable_if<false>'; did you mean 'ctype'?
   17 |     std::enable_if<(3 < 2)>::type* pointer2 = nullptr;
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      |     ctype
/opt/homebrew/Cellar/llvm/17.0.6_1/bin/../include/c++/v1/__locale:507:52: note: 'ctype' declared here
  507 | template <class _CharT> class _LIBCPP_TEMPLATE_VIS ctype;
      |                                                    ^
s3_9.cxx:17:30: error: use of class template 'std::enable_if<(3 < 2)>::ctype' requires template arguments
   17 |     std::enable_if<(3 < 2)>::type* pointer2 = nullptr;
      |                              ^
/opt/homebrew/Cellar/llvm/17.0.6_1/bin/../include/c++/v1/__locale:507:52: note: template is declared here
  507 | template <class _CharT> class _LIBCPP_TEMPLATE_VIS ctype;
      | ~~~~~~~~~~~~~~~~~~~~~~~                            ^
2 errors generated.
  > in s3_9.cxx
```

### 范例：enable_if 用于函数模板中

enable_if 典型引用是：作为函数模板的返回类型

```cxx
template <typename T>
typename std::enable_if<(sizeof(T) > 2)>::type funcEI()
/* 相当于是 void funcEI() */
{
}

int main(void)
{
    // funcEI<char>(); /* 错误：no matching function for call to 'funcEI' */
}
```

报错是：未匹配的重载函数。简单来说，就是没有找到，为什么呢？

`sizeof(char) > 2`不成立，那么也就是`typename std::enable_if<false>::type funcEI()`，
然后 false 的话，又没有 type 这个成员。然后这句话是有语法错误的`typename std::enable_if<false>::type funcEI()`；
又根据 C++ 模板的 SFINAE 特性，
`typename std::enable_if<(sizeof(char) > 2)>::type funcEI()`的实例化会被忽略。
这个时候，没有 可匹配的函数模板 或者 函数了 ---> no matching function for call to 'funcEI'

C++14 以后，写法优化了（语法糖罢了，当然也只是一个别名罢了）

`typename std::enable_if<(sizeof(char) > 2)>::type funcEI()`
---> `std::enable_if_t<(sizeof(T) > 2)> funcEI()`

go to define：

```cxx
#if _LIBCPP_STD_VER > 11
    template <bool _Bp, class _Tp = void>
    using enable_if_t = typename enable_if<_Bp, _Tp>::type;
#endif
```

### 范例：enable_if 用于类模板中

书接上回`section3 - 4 - 在构造函数模板中使用 完美转发范例`

#### 有问题的代码

```cxx
class Human {
public:
    /**
     * @brief 构造函数模板
     *
     * @tparam T
     * @param tmpName
     */
    template <typename T>
    Human(T&& tmpName)
        : m_SName(std::forward<T>(tmpName)) /* 完美转发 */
    {
        cout << "Human(T&&) 执行了" << endl;
    }

    Human(const Human& th)
        : m_SName(th.m_SName)
    {
        cout << "Human 的拷贝构造函数执行了" << endl;
    }

    Human(Human&& th)
        : m_SName(std::move(th.m_SName))
    {
        cout << "Human 的移动构造函数执行了" << endl;
    }

private:
    string m_SName;
};

int main(void)
{
    string sName = "ZhangSan";
    Human MyHuman1(sName);
    Human MyHuman2(string("LiSi"));

    // Human MyHuman3(MyHuman1); /* 错误：Boom! 主要问题是走进去了 万能引用中。
    //     我们实际上是希望调用拷贝构造，然后不可能将 MyHuman 对象 有参构造 string 对象的，而不是有参构造 */
    /* 我们现在就是想要让编译器去执行 拷贝构造函数，而不是 有参构造模板 ---> std::enable_if */

    Human MyHuman4(std::move(MyHuman1)); /* ✅ */

    const Human MyHuman5(string("Wangwu"));
    Human MyHuman6(MyHuman5); /* ✅ Human 的拷贝构造函数执行了 */
}
```

#### 改进后的代码

针对构造函数模板，如果给的参数是 string 类型，那么就让构造函数模板生效；
否则，就让构造函数模板被忽略即可。

也就是说：如果使用 enable_if 构造函数模板中，enable_if 的条件只需要设置成
`形参类型 == string类型`即可。

解决上面的问题：enable_if + is_convertible

##### is_convertible

std::is_convertible：两个模板参数分别是 Form 和 To，
用于判断能够从某个类型隐式转换为 另一个类型。返回一个 bool 值：true 或者 flase

```cxx
int main(void)
{
    cout << "string => float? " << std::is_convertible<string, float>::value << endl; // 0
    cout << "float => int? " << std::is_convertible<float, int>::value << endl; // 1
}
```

#### 代码

```cxx
/* 给类型模板定义一个别名 */
template <typename T>
using StrProcType = std::enable_if_t<std::is_convertible<T, std::string>::value>;

class Human {
public:
    /**
     * @brief 构造函数模板
     *
     * @tparam T
     * @param tmpName
     */
    // template <typename T>
    /* template <typename T,
         typename = std::enable_if_t<std::is_convertible<T, string>::value>> */
    template <typename T, typename = StrProcType<T>>
    Human(T&& tmpName)
        : m_SName(std::forward<T>(tmpName)) /* 完美转发 */
    {
        cout << "Human(T&&) 执行了" << endl;
    }

    Human(const Human& th)
        : m_SName(th.m_SName)
    {
        cout << "Human 的拷贝构造函数执行了" << endl;
    }

    Human(Human&& th)
        : m_SName(std::move(th.m_SName))
    {
        cout << "Human 的移动构造函数执行了" << endl;
    }

private:
    string m_SName;
};

int main(void)
{
    string sName = "ZhangSan";
    Human MyHuman1(sName);

    Human MyHuman3(MyHuman1); // Human 的拷贝构造函数执行了
}
```

这里的几种技巧：

1. 匿名的模板参数

```cxx
template <typename T,
    typename = std::enable_if_t<std::is_convertible<T, string>::value>>
```

这里的`typename = xxx`，这里应该是`typename U = xxx`，只不过这里的 U 在下面代码中用不到，省去了

2. using 别名模板

```cxx
template <typename T>
using StrProcType = std::enable_if_t<std::is_convertible<T, std::string>::value>;

template <typename T,
    typename = StrProcType<T>>
Human(T&& tmpName) { }
```

3. std::enable_if_t

这里如果是 false，那么就有一个语法错误`tmeplate<typename T, typename = <空>`，
因为`eanble_if::type`不存在，那么根据 C++ 的 SFINAE 特性，不过不匹配的调用就会被忽略
