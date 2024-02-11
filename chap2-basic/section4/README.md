# chap2_4 - 可变参模板

可变参模板：variadic templates（C++11 以后）

允许模板定义中包含 0 到多个（任意个）模板参数。

## 1 - 可变参函数模板

### 基本含义

```cxx
/**
 * @brief
 *
 * @tparam T 一包类型
 * @param args 一包形参（一堆 参数）（函数模板的形参：0~多），每个参数的类型可以各不相同
 * 参数数量 与 类型数量必定是相同的
 * ... 代表 模板参数包
 * T 称为 可变参类型名，T实际上可以是 0～多 不同的类型，实际上是一包类型
 * T... 用来修饰 args
 */
template <typename... T>
void myvtfunct(T... args)
{
    cout << "---------- begin ----------" << endl;
	/*
	 *	sizeof... 固定语法，C++11 中引入，用于表示 收到的模板参数个数 或者 类型数量
	 *  sizeof... 针对的，只能是这种...的可变参，后面圆括号中可以是 函数模板的形参 args，也可以是类型模板参数 T
	 */
    cout << sizeof...(args) << endl;
    cout << sizeof...(T) << endl;
    cout << "---------- end ----------" << endl;
}

int main(void)
{
    myvtfunct();

    /* 完全不指定类型 */
    myvtfunct(10, 20, 30, 40, 'a', 'b', "abc", myvtfunct<int>);

    /* 指定部分类型 */
    myvtfunct<double, double>(10, 20, 30, 40, 'a', 'b', "abc", myvtfunct<int>);
}
```

形参包展开的形式是比较固定的，一般采用递归的形式展开形参包。
要展开，要求：1. 形参包展开函数，2. 一个同名的递归终止函数

```cxx
/* 同名的递归终止函数（这是一个普通函数，而不是函数模板），必须放在 “展开函数” 的 上面 */
/* 否则：Call to function 'myvtfunct' that is neither visible in the template definition nor found by argument-dependent lookup */
void myvtfunct()
{
    cout << "参数包展开时执行了递归终止函数 myvtfunct()" << endl;
}

/**
 * @brief 形参包展开函数
 *
 * @tparam T
 * @tparam U
 * @param firstArg
 * @param otherArgs
 */
template <typename T, typename... U>
void myvtfunct(T firstArg, U... otherArgs)
{
    cout << "收到的参数值: " << firstArg << endl;
    myvtfunct(otherArgs...); /* 递归调用，注意：塞进来的是一包形参，这里的...不能省略 */
}

int main(void)
{
    myvtfunct(10, 20, 12.7, 'c', "fuck", myvtfunct<int>);
}
```

当然，在 C++17 中增加了一种语句，叫做 “编译 if”（if constexpr），加一个递归的 base case

```cxx
template <typename T, typename... U>
void myvtfunct(T firstArg, U... otherArgs)
{
    if constexpr (sizeof...(U) > 0) {
        cout << "收到的参数值: " << firstArg << endl;
        myvtfunct(otherArgs...); /* 递归调用，注意：塞进来的是一包形参，这里的...不能省略 */
    } else { ... }
}

int main(void)
{
    myvtfunct(10, 20, 12.7, 'c', "fuck", myvtfunct<int>);
}
```

#### 深入理解 if constexpr ()

1. 不满足条件的分支，依然会被编译器编译（被编译器进行语法检查），这与条件编译不一样`#if #else #endif`（就是条件编译，不会检查不满足条件的语法）。

2. `if constexpr ()`里面指定的必须是常量：理解成普通 if 语句，只是判断条件从 执行期间 挪到了 编译期间。

```cxx
int i = 8;
if constexpr (i > 8) { /* ... */ }  // 错误
```

### 重载

可变参函数模板也是可以重载的

```cxx
template <typename... T>
void myfunc(T... arg)
{
    cout << "myfunc(T... arg) 执行了!" << endl;
}

template <typename... T>
void myfunc(T*... arg)
{
    cout << "myfunc(T*... arg) 执行了!" << endl;
}

void myfunc(int arg)
{
    cout << "myfunc(int arg) 执行了!" << endl;
}

int main(void)
{
    myfunc(NULL); // myfunc(T... arg) 执行了!
    myfunc((void*)0); // myfunc(T*... arg) 执行了!
    myfunc(nullptr); // myfunc(T... arg) 执行了!
    myfunc((int*)nullptr); // myfunc(T*... arg) 执行了!
    cout << typeid(NULL).name() << endl; // l
    cout << sizeof(NULL) << endl; // 8
}
```

## 2 - 折叠表达式

折叠表达式：flod expression

目的：计算某个值（表达式的结果当然是一个值）

该值的特殊性在于：他与所有的可变参有关，而不是与单独某个可变参有关。
需要所有的可变参都参数计算才能求出该值。

注意，每种格式的表达式都要用圆括号扩着

左折：圆括号偏左，从左侧开始计算，`...`在左边

### 一元左折（unary left fold）

```cxx
template <typename... T>
auto add_val(T... args)
{
    // (((1 + 2) + ...) + n )
    return (... + args); /* 折叠表达式，圆括号不能省略 */
}

int main(void)
{
    cout << add_val(10, 20, 30, 30.1) << endl;
}
```

下面是 C++ insight 解释的版本：

```cxx
template <>
double add_val<int, int, int, double>(int __args0, int __args1, int __args2,
    double __args3)
{
	/* 参数从左侧开始先计算 */
    return (static_cast<double>((__args0 + __args1) + __args2)) + __args3;
}
```

### 一元右折

```cxx
template <typename... T>
auto unary_right(T... args)
{
	// ( 1 + (... + (n-1 + n)))
    return (args + ...);
}
```

```cxx
template <>
int unary_right<int, int, int, int>(int __args0, int __args1, int __args2,
    int __args3)
{
    return __args0 + (__args1 + (__args2 + __args3));
}
```

### 二元左折 (binary left fold)

格式：`(init + ... + args)`

计算方式：`((((init) + 1) + ... ) ) + n)`

init 表示一个初始的东西，他可能是一个值，也可能是一个其他东西

```cxx
template <typename... T>
auto binary_left(T... args)
{
    (cout << ... << args);
}

int main(void)
{
    binary_left("10\n", "abc\n", "def\n");
    /**
     *  10
     *  20
     *  abc
     *  def
     */
}
```

下面是 C++ insight 的解释

```cxx
template <>
void binary_left<const char*, const char*, const char*>(
    const char* __args0, const char* __args1, const char* __args2)
{
    std::operator<<(std::operator<<(std::operator<<(std::cout, __args0), __args1),
        __args2);
}
```

### 二元右折

格式：`(args + ... + init)`

计算方式`(1 + (... + (n + init)))`

二元右折 与 二元左折类似，不过是 计算顺序改变了

## 3 - 可变参表达式

已有下面的折叠表达式

```cxx
template <typename... T>
auto print_result(T const&... args)
{
    (cout << ... << args) << "结束" << endl;
    return (... + args);
}

int main(void) {
	print_result(10, 20, 30, 40);
}
```

输出：`10203040`

需求：想要让中间过程就是翻倍的，即`20406080`。
要解决这个问题，比较好的方式就是：引入一个中间函数模板

```cxx
template <typename... T>
auto print_result(T const&... args)
{
    (cout << ... << args) << "结束" << endl;
    return (... + args);
}

template <typename... T>
void print_calc(T const&... args)
{
    cout << print_result(2 * args...) << endl;
}

int main(void) {
	print_calc(10, 20, 30, 40);
}
```

这个`print_calc`中间的`(2 * args ...)`就是一个折叠表达式。

下面是 C++ insight 给出的结果

```cxx
template <>
int print_result<int, int, int, int>(const int& __args0, const int& __args1,
    const int& __args2, const int& __args3)
{
    std::operator<<(
        std::cout.operator<<(__args0).operator<<(__args1).operator<<(__args2).
        operator<<(__args3),
        "\347\273\223\346\235\237")
        .
        operator<<(std::endl);
    return ((__args0 + __args1) + __args2) + __args3;
}

template <>
void print_calc<int, int, int, int>(const int& __args0, const int& __args1,
    const int& __args2, const int& __args3)
{
    std::cout
        .
        operator<<(
            print_result(2 * __args0, 2 * __args1, 2 * __args2, 2 * __args3))
        .
        operator<<(std::endl);
}
```

这个`(2 * args ...)`就相当于是：每个参数都 乘以 2

## 4 - 可变参 类模板

允许模板定义中包含 0 ～多个（任意个） 模板参数

参数包展开方式有多重，下面是介绍典型的展开方式

### 通过递归继承方式展开 类型、非类型、模板模板形参包

递归继承 就是一种典型的展开方式，非常常见。

```cxx
// 主模板定义（泛化版本的类模板）
template <typename... Args>
class myClassT { };

// // 也有的人不写定义，只写声明（主模板）
// template <typename... Args>
// class myClassT; /* 这个只写声明能正确，是因为没有用 泛化版本 创建对象。但是可以使用偏特化、特化创建对象 */

template <typename First, typename... Others>
class myClassT<First, Others...> : private myClassT<Others...> // 偏特化
{
};
```

下面是代码：

```cxx
// 主模板定义（泛化版本的类模板）
template <typename... Args>
class myClassT {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 泛化版本执行了, this=%p\n", this);
    }
};

template <typename First, typename... Others>
class myClassT<First, Others...> : private myClassT<Others...> // 偏特化
{
public:
    myClassT()
        : m_i(0)
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%d\n", this, sizeof...(Others));
    }

    First m_i;
};

int main(void)
{
    myClassT<int, float, double> myc;
    /**
     * myClassT::myClassT() 泛化版本执行了, this=0x16f53eaf0
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=0
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=1
     * myClassT::myClassT() 偏特化版本执行了, this=0x16f53eaf0, sizeof...(Others)=2
     */
}
```

如何理解，首先：`class myClassT<First, Others...>`必须要满足：`1 + x(x>0)`.
每次都是继承上`1 + x - 1`个参数的版本，最后如果`x == 0`，那么就是继承`private myClassT<>`，
这个时候只有`template <typename... Args> class myClassT`这个版本满足。
当然也可以写一个特化的版本（因为只是特化版本，因此老祖宗`template <typename... Args> class myClassT`不能删除），
也就是没有参数的版本`template<> class myclassT<>{};`。
当然，这个看起来像是全特化，但是实际上不是全特化（可变参数模板不存在全特化）

上面使用`private`继承，有一个很重要的原因就是：子类字段 与 父类字段 可能会重名。

一次只继承一个 少一个参数的。

1. 不能带有多个 可变模板参数

   不能出现这种代码：`template <typename... First, typename... Others>`

2. 可变模板参数只能放到 最后写

   不能出现这种代码：`template <typename... First, typename Others>`

#### 类型模板参数包的展开

```cxx
template <typename... Args>
class myClassT {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 取材于 C++ 标准库的 tuple
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class myClassT<First, Others...> : private myClassT<Others...> // 偏特化
{
public:
    myClassT()
        : m_i(0)
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu\n", this, sizeof...(Others));
    }

	/* 有参构造 */
    myClassT(First parf, Others... paro)
        : m_i(parf)
        , myClassT<Others...>(paro...) /* 传递给父类的构造函数 */
    {
        cout << "---------- begin ----------" << endl;
        printf("myclassT::myclassT(parf, ...paro)执行了, this=%p\n", this);
        cout << m_i << endl;
        cout << "---------- end ----------" << endl;
    }

    First m_i;
};
```

#### 非模板类型模板参数包

```cxx
template <int... FTArgs>
class myClassT {
    myClassT()
    {
        printf("myClassT::myClassT()泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 偏特化
 *
 * @tparam First
 * @tparam Others
 */
template <int First, int... Others>
class myClassT<First, Others...> : private myClassT<Others...> {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu, First=%d\n", this, sizeof...(Others), First);
    }
};
```

#### 模板模板参数包的展开

```cxx
/**
 * @brief 泛化版本的 模板模板参数
 *
 * @tparam T
 * @tparam Container...
 */
template <typename T,
    template <typename> typename... Container // 一包
    >
class ParentMM {
public:
    ParentMM()
    {
        printf("ParentMM::ParentMM() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 偏特化
 *
 * @tparam T
 * @tparam FirstContainers
 * @tparam OthersContainers...
 */
template <typename T,
    template <typename> typename FirstContainers, // 一个
    template <typename> typename... OthersContainers // 一包
    >
class ParentMM<T, FirstContainers, OthersContainers...>
    : private ParentMM<T, OthersContainers...> // 继承
{
public:
    ParentMM()
        : m_containers(vector<T>())
    {
        printf("ParentMM::ParentMM() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu\n", this, sizeof...(OthersContainers));
        m_containers.push_back(12);
    }

    FirstContainers<T> m_containers;
};

template <typename T,
    template <typename> typename... Container // 一包
    >
class myClassT : public ParentMM<T, Container...> {
public:
    myClassT()
    {
        /* boost 中的 type_id_with_cvr<...>().pretty_name() 甚至可以显示万能引用的类型 */
        printf("myClassT::myClassT() 执行了, this=%p, T的类型是: %s, Container 参数个数是 %d 个\n",
            this, typeid(T).name(), sizeof...(Container));
    }
};
```

很奇怪，不知道为什么我的代码有问题（虽然 gpt 写的也有问题）

### 通过递归组合方式展开参数包

什么叫做组合关系，组合关系也称为 符合关系，实际上是 一种包含关系。
当然，上面的 private 继承也是一种组合关系。

继承的话，相当于是将成员放到了同一个类中。

 <!-- TODO 需要用 pahole 测试一下 -->

```cxx
template <typename... Args>
class myClassT {
public:
    myClassT()
    {
        printf("myClassT::myClassT() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 取材于 C++ 标准库的 tuple
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class myClassT<First, Others...> // : private myClassT<Others...> // 偏特化
{
public:
    myClassT()
        : m_i(0)
    {
        printf("myClassT::myClassT() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu\n", this, sizeof...(Others));
    }

    /**
     * @brief Construct a new my Class T object 有参构造
     *
     * @param parf
     * @param paro
     */
    myClassT(First parf, Others... paro)
        : m_i(parf)
        , m_o(paro...)
    // , myClassT<Others...>(paro...) /* 传递给父类的构造函数 */
    {
        cout << "---------- begin ----------" << endl;
        printf("myclassT::myclassT(parf, ...paro)执行了, this=%p\n", this);
        cout << m_i << endl;
        cout << "---------- end ----------" << endl;
    }

    First m_i;
    myClassT<Others...> m_o;
};

int main(void)
{
    myClassT<int, int, double, float>(10, 20, 2.1, 2.3f);
    /*
            myClassT::myClassT() 泛化版本执行了, this=0x16fd96d1c
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d18
                        2.3
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d10
                        2.1
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d08
                        20
                        ---------- end ----------
                        ---------- begin ----------
                        myclassT::myclassT(parf, ...paro)执行了, this=0x16fd96d00
                        10
                        ---------- end ----------
    */
}
```

### 通过 tuple 和 递归调用展开参数包

1. tuple 的概念和简单演示

tuple: 元组，是一个可变参类模板

例一：

```cxx
int main(void) {
	tuple<double, int, int> mt(12.5, 100, 52);
	cout << get<0>(mt) << endl;
	cout << get<1>(mt) << endl;
	cout << get<2>(mt) << endl;
}
```

例二（函数参数的 tuple）：

```cxx
template <typename... T>
void myfuncTuple(const tuple<T...>& t) { }
```

2. 如何用 tuple 和递归调用展开参数包

有一个计数器变量，从 0 开始，每处理一个参数，计数器++，一直到把所有参数处理完。
最后，提供一个模板偏特化，作为递归调用结束

```cxx
/**
 * @brief 泛化版本
 *
 * @tparam cnt 用于统计
 * @tparam maxCNT 参数的数量
 * @tparam T
 */
template <int cnt, int maxCNT, typename... T>
class myClassT {
public:
    static void sfunc(const tuple<T...>& t)
    {
        cout << "value = " << get<cnt>(t) << endl; /* 可以把每个参数都取出来并输出 */
        myClassT<cnt + 1, maxCNT, T...>::sfunc(t); /* 递归调用 */
    }
};

/**
 * @brief 偏特化版本，用于结束递归调用
 *
 * @tparam maxCNT
 * @tparam T
 */
template <int maxCNT, typename... T>
class myClassT<maxCNT, maxCNT, T...> /* 通过这个尖括号，反推上面参数 */ {
public:
    static void sfunc(const tuple<T...>& t)
    {
        /* 这里保持为空就行了，实际上，这个 maxCNT 正好是到了这个越界位置了 */
    }
};

/**
 * @brief 包装一下
 *
 * @tparam T
 * @param t
 */
template <typename... T>
void myfuncTuple(const tuple<T...>& t)
{
    myClassT<0, sizeof...(T), T...>::sfunc(t);
}

int main(void)
{
    tuple<double, int, int> mt(12.5, 100, 52);
    myfuncTuple(mt);
}
```

### 基类参数包的展开

```cxx
template <typename... myCLSPList>
class myClassT : public myCLSPList... // 有一堆爹
{
public:
    myClassT()
        : myCLSPList()... /* 一堆爹用这种方式伺候 */
    {
        printf("myClassT::myClassT, this=%p\n", this);
    }
};

class PA1 {
public:
    PA1()
    {
        printf("PA1:PA1, this=%p\n", this);
    }

private:
    char m_s1[100];
};

class PA2 {
public:
    PA2()
    {
        printf("PA2:PA2, this=%p\n", this);
    }

private:
    char m_s2[200];
};

class PA3 {
public:
    PA3()
    {
        printf("PA3:PA3, this=%p\n", this);
    }

private:
    char m_s3[300];
};

int main(void)
{
    myClassT<PA1, PA2, PA3> Obj;
    /*
            PA1:PA1, this=0x16fc36aa0 === myClassT （ this 指针调整 )
            PA2:PA2, this=0x16fc36b04
            PA3:PA3, this=0x16fc36bcc
            myClassT::myClassT, this=0x16fc36aa0
    */
    cout << "sizeof(Obj) = " << sizeof(Obj) << endl; /* sizeof(Obj) = 600 */
}
```

### 特化

可变参模板不存在全特化，所以只有偏特化

代码请看`s4_13.cxx`
