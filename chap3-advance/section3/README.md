# chap3_3 - 完美转发

## 1 - 完美转发的概念和步骤演绎

### 概念

几个概念：

1. 直接调用`funcLast();`：直接调用，不会被一个中间函数包装

2. 转发：通过 funcMiddle() 间接调用 funcLast()，funcMIddle 相当于一个跳板函数。
   如果有参数，那么参数也需要通过 funcMiddle 中转 传递给 funcLast（也就是调用 funcLast 是最终目的）

   在中转的时候，一些信息就会丢失，比方说：传值 就会丢掉`const`、`&`之类的。
   以及，如果是`funcMiddle(10)`，这个 10 就是一个右值，但是经过参数推导，然后`funcMiddle`最终接受的就是一个左值，
   这个就是属性丢失了

3. 完美转发：把收到的参数以及这些参数相对应的类型不变的转发给其他函数

   const、左值、右值 这些属性信息原封不动

一般在写跳板函数的时候，是一个函数模板，后面会说明 完美转发如何用

### 范例演示

下面这些范例：先解释了转发，然后转发函数模板（funcMiddle_Temp）应该选用什么参数。

先来了一个转发的例子`传值`，然后传值的弊端；再来了万能引用（看起来更加的正确），但是还不太正确

#### 传值

```cxx
void funcLast(int v1, int v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

/**
 * @brief
 *
 * @tparam F 函数指针类型
 */
template <typename F, typename T1, typename T2>
void funcMiddle_Temp(F f, T1 t1, T2 t2)
{
    f(t1, t2);
}

int main(void)
{
    int i = 50;
    funcLast(41, i);

    int j = 70;
    funcMiddle_Temp(funcLast, 20, j); // 91
    cout << j << endl; // 70
}
```

#### 传值转发的弊端

```cxx
void funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

/**
 * @brief
 *
 * @tparam F 函数指针类型
 */
template <typename F, typename T1, typename T2>
void funcMiddle_Temp(F f, T1 t1, T2 t2)  // 这个 t2 被推断为 int 类型，而不是 int& 类型
{
    f(t1, t2);
}

int main(void)
{
    int i = 50;
    funcLast(41, i); // 92

    int j = 70;
    funcMiddle_Temp(funcLast, 20, j); // 91
    cout << j << endl; // 70
}
```

这个中转函数（模板）`template <typename F, typename T1, typename T2> void funcMiddle_Temp(F f, T1 t1, T2 t2)`就不透明，改变了 funcLast 的语意。这个 t2 被推断为 int 类型，而不是 int& 类型

#### 万能引用转发

万能引用可以保存修饰的所有类型。

实参的所有信息都会传递到万能引用当中去，从而让编译器退到出来函数模板最终的形参类型（引用折叠）

```cxx
void funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

template <typename F, typename T1, typename T2>
void funcMiddle_Temp(F f, T1&& t1, T2&& t2)
{
    f(t1, t2);
}

#ifdef INSIGHTS_USE_TEMPLATE
template <>  // 这个是用了万能引用之后的
void funcMiddle_Temp<void (*)(int, int&), int, int&>(void (*f)(int, int&),
    int&& t1, int& t2)
{
    f(t1, t2);
}
#endif

int main(void)
{
    int i = 50;
    funcLast(41, i); // 92

    int j = 70;
    funcMiddle_Temp(funcLast, 20, j); // 91
    cout << j << endl; // 70
}
```

万能引用比`传值`的版本更加的正确，但是也不一定就是最准确的。

#### 右值引用类型 是 左值（万能引用转发的弊端）

```cxx
/**
 * @brief 万能引用失败的情况 ---> 右值引用
 *
 * @param v1
 * @param v2
 */
void funcLast2(int&& v1, int& v2)
{
    cout << v1 << endl;
    cout << v2 << endl;
}

template <typename F, typename T1, typename T2>
void funcMiddle_Temp(F f, T1&& t1, T2&& t2)
{
    f(t1, t2);
}

int main(void)
{
    // int i = 50;
    // funcLast(41, i); // 92

    // int j = 70;
    // funcMiddle_Temp(funcLast, 20, j); // 91
    // cout << j << endl; // 70

    int j = 70;
    funcLast2(20, j); // 20 70

    funcMiddle_Temp(funcLast2, 20, j);
    /**
     * f(t1, t2) : 错误: rvalue reference to type 'int' cannot bind to lvalue of type 'int'
     *   ^~
     * 首先 t1 是右值引用类型，但是 t1 本身是左值，不能将左值传递给右值引用
     */
}
```

我们现在就是想要将`t1`这个右值引用类型（左值），拧回成原本的`20`，也就是一个纯右值。

完美转发：就是让程序员可以书写：接受任意实参的函数模板（funcMiddle_Temp），
并将其转发到目标函数（funcLast2），目标函数会接收到与 转发函数（funcMiddle_Temp）所接受的完全相同
（当然包括类型相同比如保持参数的左值、右值特性）的参数。要实现完美转发，就要用到`std::forward`了。

## 2 - std::forward

C++11 中专门为转发而存在的函数。这个函数要么返回一个左值，要么返回一个右值。

万能引用类型才是 forward 能够发挥作用的重要条件。
（有 forward 就一定有万能引用）

右值引用类型，能接受右值是极好的，但是右值引用却是一个 左值，
也就是我们上面，万能引用的形参，都是左值了。主要是拧回去

理解：

1. 实参原来是个左值 j，到了形参中还是左值 t2。forward 能够转化回原来该实参的左值或者右值性。
   所以，forward 之后还是个左值

2. 实参原来是一个右值 20，到了实参中编程了左值 t1。forward 能够转化回原来改实参的左值 或者 右值性。
   所以，forward 之后还是个右值。forward 这个函数有强制把左值转化为右值的能力。
   所以，forward 这个函数只对原来是个右值这种情形有用

### 书接上回：万能引用

```cxx
template <typename t>
void func(t&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "t=" << type_id_with_cvr<t>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

int main(void) {
    int i = 18;
    func(i);
    // /**
    //  * T=int&
    //  * tmprv=int&
    //  *
    //  */


    func(100);
    // /**
    //  * T=int
    //  * tmprv=int&&
    //  */
}
```

万能引用：如果 实参是`int&`，那么`T=int&, tmprv=int&`；如果 实参是纯右值，那么`T=int, tmprv=int&&`

### 完美转发写法

```cxx
void funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

template <typename F, typename T1, typename T2>
void funcMiddle_Temp2(F f, T1&& t1, T2&& t2)
{
    f(
        std::forward<T1>(t1), // T1=int
        std::forward<T2>(t2) // T2=int&
    );
}
```

forward 函数：

1. 如果传递的是`int`或者是`int&&` ---> 右值
2. 如果传递的是`int&` ---> 左值

### 完美转发的小例子

```cxx
void printInfo(int& t)
{
    cout << "printInfo() 参数类型是 左值引用" << endl;
}

void printInfo(int&& t)
{
    cout << "printInfo() 参数类型是 右值引用" << endl;
}

template <typename T>
void TestF(T&& t)
{
    printInfo(
        std::forward<T>(t));
}

int main(void)
{
    int i = 100;
    TestF(10);
    /**
     * printInfo() 参数类型是 右值引用
     */
    TestF(i);
    /**
     * printInfo() 参数类型是 左值引用
     */
    TestF(std::move(i));
    /**
     * printInfo() 参数类型是 右值引用
     */
    return 0;
}
```

### 非引用参数的完美转发

```cxx
void funcLast3(int v1)
// void funcLast3(int&& v1) /* 当然也是可以哒，只不过这里的例子是 非引用参数 */
{
    cout << "v1=" << v1 << endl;
}

void funcMiddle_Temp()
{
    auto&& result = getData(); /* 万能引用，返回的是将亡值 */
    // getData() = 10; /* 错误：expression is not assignable，是不是左值，看看能不能放到等号左边 */

    result += 10; /* 模拟了对 result 的各种计算 */

    funcLast3(
        /* decltype 可以推断出类型 */
        std::forward<decltype(result)>(result));
}
```

## 4 - 在构造函数模板中使用 完美转发范例

原始的例子：

```cxx
#include <iostream>

using namespace std;

class Human {
public:
    /* 构造函数 */
    Human(const string& tmpName)
        : m_SName(tmpName)
    {
        cout << "Human(const string& tmpName) 执行了" << endl;
    }

    // Human(string&& tmpname) : m_SName(tmpname)
    Human(string&& tmpname)
        : m_SName(std::move(tmpname)) /* move 只是将左值变成一个右值，并不具有移动能力 */
    /* 这里就是会执行 string 的移动构造函数（接受右值） */
    /* 将 tmpName 移走，这个不是 std::move 的行为，而是 string 的移动构造函数做的 */
    /* 这个时候 tmpName 就空了 */
    {
        cout << "Human(string&& tmpName) 执行了" << endl;
    }

private:
    string m_SName;
};

int main(void)
{
    string sName = "ZhangSan";
    Human MyHuman1(sName); // Human(const string& tmpName) 执行了
    Human MyHuman2("LiSi"); // Human(string&& tmpName) 执行了
}
```

我们现在就是要将`Human(String&&)`和`Human(const String& )`合二为一。

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

## 5 - 在 可变参 模板中使用完美转发范例

### 常规的 在可变参模板使用完美转发

这个写法比较复杂，记一下就行了

```cxx
template <typename F, typename... T>
void funcMiddle_Temp(F f, T&&... t)
{
    f(std::forward<T>(t)...);
}

void funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
}

int main(void)
{
    int j = 70;
    funcMiddle_Temp(funcLast, 20, j); // 91
    cout << "j=" << j << endl; // 71
}
```

### 将目标函数中的返回值 通过转发函数返回 给调用者函数

用到的技术：auto + decltype 构成返回类型的后置语法

```cxx
template <typename F, typename... T>
/* FIXME 这里的 decltype 不会浪费资源，decltype 与 sizeof 都是不求值的 */
auto funcMiddle_Temp(F f, T&&... t) -> decltype(f(std::forward<T>(t)...))
{
    return f(std::forward<T>(t)...);
}

int funcLast(int v1, int& v2)
{
    ++v2;
    cout << v1 + v2 << endl;
    return v1 + v2;
}

int main(void)
{
    int j = 70;
    int k = funcMiddle_Temp(funcLast, 20, j); // 91
    cout << "j=" << j << endl; // 71
    cout << "k=" << k << endl; // 91
}
```

但是这种写法会出现，引用的丢失的问题`int& ---> int`，可以使用下面这种写法

```cxx
template <typename F, typename... T>
/* FIXME 这里的 decltype 不会浪费资源，decltype 与 sizeof 都是不求值的 */
decltype(auto) funcMiddle_Temp(F f, T&&... t)
{
    return f(std::forward<T>(t)...);
}
```

## 6 - 完美转发失败的情形（例子）

使用 NULL 或者 0 作为空指针进行参数传递时，导致完美转发失败的情况

```cxx
template <typename F, typename... T>
void funcMiddle_Temp1(F f, T&&... t)
{
    f(std::forward<T>(t)...);
}

int main(void)
{
    // funcMiddle_Temp1(funcLast4, NULL);
    // /* 错误：f(std::forward<T>(t)...); ：Cannot initialize a parameter of type 'char *' with an rvalue of type 'long' */
    // /* 解决方案：使用nullptr */

    funcMiddle_Temp1(funcLast4, nullptr);
}
```
