# chap3_2 - 理解函数模板类型推断、查看推断结果、auto 类型推断

## 1 - 如何查看类型推断结果

当前要解决的问题：如何查看类型推断结果 —— 编译器给我们进行类型推断的结果
（要求：掌握 C++ 类型推断的规则）（比较靠谱的方法：依赖 boost 库 —— 如何利用这个库打印出编译器推断出来的类型信息）

```cxx
#include <boost/type_index.hpp>

template <typename T>
void myfunc(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}
```

## 2 - 理解函数模板类型推断

```cxx
    myfunc(100);
    /**
     * template <typename T> void myfunc(const T& tmprv) 可以通过
     * 但是 template <typename T> void myfunc(T& tmprv) 不能通过
     * 输出：
     * T=int
     * tmprv=int const&
     */
```

myfunc 中的 T 类型不仅仅取决于实参 100，还取决于 temprv 的类型

看一下，函数模板的形参(tmprv)是如下类型是，编译器推断出的 类型模板参数的类型(T)，以及最终的函数形参`tmprv`类型

这里有两个类型：

1. `T` 的类型
2. 形参的类型

### 引用或指针类型

指的是：`myfunc(const T& tmprv)`也就是`const T&`这一块内容是指针 或者 引用类型的时候，
学习编译器是如何推断的

```cxx
template <typename T>
void myfunc(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void)
{
    int i = 1;
    const int j = i;
    const int& k = i;
    myfunc(i);
    /**
     * T = int
     * tmprv = int &
     */
    myfunc(j);
    /**
     * T=int const
     * tmprv=int const&
     */
    myfunc(k); // 如果实参是一个，引用的部分会被忽略
    /**
     * T=int const
     * tmprv=int const&
     */
}
```

#### 函数模板，形参是引用

```cxx
template <typename T>
void myfunc(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;

    tmprv = 12;
}

int main(void)
{
    int i = 1;
    myfunc(i);
    cout << i << endl; // 12
}
```

#### 函数模板，形参不是引用，但是传入引用

```cxx
template <typename T>
void myfunc2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;

    tmprv = 18;
}

int main(void)
{
    int i = 1;
    int& j = i;
    myfunc2(j);
    cout << j << endl;
    /**
     * T=int
     * tmprv=int
     */
}
```

这里例子中，不会因为 j 是 引用类型，就将 T 推导为引用类型，
这里的 引用符号 就被忽略掉了

#### 函数模板，形参引用，实参 const 引用

```cxx
template <typename T>
void myfunc3(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
    // tmprv = 12; /* 这会导致错误，因为传入的是 const int & */
}

template <typename T>
void myfunc4(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void)
{
    int i = 1;
    const int& k = i;
    myfunc3(k);
    /**
     * T=int const 实参是引用类型，引用类型会被忽略掉（T不会被推导为引用类型）
     * tmprv=int const&
     */
    myfunc4(k);
    /**
     * T=int
     * tmprv=int
     */
}
```

#### 函数模板，形参是指针

```cxx
template <typename T>
void myfunc6(T* tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void) {
    int i = 89;
    const int* pi = &i;
    myfunc6(&i);
    /**
     * T=int
     * tmprv=int*
     */

    myfunc6(pi);
    /**
     * T=int const
     * tmprv=int const*
     */
}
```

#### 小结一下（重要）

得到一些结论：

1. 如果实参是一个引用类型，引用部分会忽略，也就是 T 的类型是忽略了引用（如果形参`void func(T& tmprv)`是带有引用的话）
2. 当向引用类型的形参 tmprv 传入 const 类型实参时，形参会称为 const 引用（原来是个引用），
   这里可以看到，实例的 const 树形会称为类型模板参数 T 类型推导的组成部分，所以不用担心在 func 中能够修改原来 const 属性的实参

   1. 如果实参是引用类型，T 的引用类型会被忽略掉（T 不会被推到为引用类型）
   2. 只要形参中带了 const，那么 T 中的 const 就没了（T 的类型不仅仅取决于实参，还取决于 tmprv 的类型）

3. 通过上面的范例，小结一下编码技巧：

   1. 形参中引用有两个作用
   2. 传递引用比传递值效率高

   所以，一般写函数模板的时候，形参建议优先考虑`T &tmprc`这样的形态。
   这样的形参就不怕形参中的引用被忽略掉，而导致开发者想通过对形参的修改达到实参的本意 无法达成

4. 既想要享受引用带来的效率上的提高，又不希望通过形参来修改实参，则函数模板中的形参建议考虑为`const T & tmprv`这样的形参

5. 如果 tmprv 是个指针（形参是指针）

   1. 则实参中的 const 会被代入到 类型 T 中去，如果 tmprv 中有 const，则 T 类型中不会带 const
   2. (1) 这个结论对于 func 形参为`T& tmprv`或者`const T& tmprv`也适用

#### 我的小结

1. const

   1. 形参有 const，无论实参是否有 const，直接塞入 const
   2. 形参无 cosnt，需要看 实参

2. 传值：一律按照传值处理，即便实参是引用
3. 传引用：那么就是引用
4. 传指针：相当于是一种约束了，只能传入指针

### 万能引用类型

```cxx
template <typename T>
void func(T&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void)
{

    int i = 18;
    func(i);
    /**
     * T=int& 典型
     * tmprv=int&
     *
     */

    int j = i;
    func(j);
    /**
     * T=int const&
     * tmprv=int const&
     */

    const int& k = i;
    func(k);
    /**
     * T=int const&
     * tmprv=int const&
     */

    func(100);
    /**
     * T=int
     * tmprv=int&& 典型
     */
}
```

1. 如果传入左值，那么 tmprv 和 T 推断出来左值（`int&`）
2. 如果传入右值，那么 T 推断出来为 int；tmprv 推断出来 int&&

### 形参传值：实参非指针、实参是指针

传进去的是一个拷贝，是对象的副本

实参是指针，既有 传值的特点（p 的指向并不是 const），也有引用的特点（指向的内存的 const）

```cxx
template <typename T>
void func2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void) {
    int i = 18;
    func2(i); /**
     * T=int
     * tmprv=int
     */

    int& k = i;
    func2(j);
    /**
     * T=int
     * tmprv=int
     */

    const int& k = i;
    func2(k);
    /**
     * T=int
     * tmprv=int
     */

    func2<int&>(i);
    /**
     * T=int&
     * tmprv=int&
     */

    func2(&i);
    /**
     * T=int*
     * tmprv=int*
     */

    char p[] = "fuck";
    func2(p);
    /**
     * T=char*
     * tmprv=char*
     */

    const char* cp = p;
    func2(cp);
    /**
     * T=char const*
     * tmprv=char const* 这个实际上是 const char * 的意思，只不过这里写的有点奇怪
     */

    char* const cpc = p;
    func2(cpc);
    /**
     *
     * T=char*
     * tmprv=char* 这种，首先，传值会忽略 const，然后这里就只是 char * 类型了
     */
}
```

1. 若引用实参是引用类型，则引用部分会被忽略掉，T 不会被 推到为引用类型。除非手动指定（当然一般不会怎么写代码`func2<int&>`）
2. 如果实参是 const，则 const 部分会被忽略，也就是 T 不会被推到为 const 类型
3. 如果传入的类型是指针，那么 T 就是一个指针类型：
   - 如果传入的带有 const，实际上保留的都是第一个 const

两个 const:

1. 第一个 const 表示：p 指向的内容不能被 p 改变
2. 第二个 cosnt 表示：p 不能再指向其他内容。

利用`typeid(<variable>).name()`得到的是简称：PKc(pointer const char，喵的，这种缩写太搞了)

### 传值方式的引申 - std::ref 与 std::cref

如果用了这两个东西，那么就变成了 引用传递了

#### std::ref 的原理

这个`std::ref`和`std::cref`像：对象包装器。
编译器通过创建一个`class std::reference_wrapper<T>类型的对象`。

也就是`func(std::ref(m))`的`func`接受的是：`std::reference_wrapper<T>`对象

```cxx
template <typename T>
void func2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;

    // tmprv = 12; /* 错误：未定义 operator= */
    int& tmpvaluec = tmprc;
    tmpvalue = 1200;  /* ✅ */
}

int main(void) {
    int m = 180;
    func2(m);
    func2(std::ref(m));
    /**
     * T=std::__1::reference_wrapper<int>
     * tmprv=std::__1::reference_wrapper<int>
     */
}
```

但是即使是`std::ref(m)`

当函数模板形参：传值，可以通过`std::ref`来引用传参（这简直就是抖机灵）

### 数组做实参

```cxx
template <typename T>
void func(T&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

template <typename T>
void func2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

template <typename T>
void func3(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

int main(void) {
    const char str[] = "fuck you";
    func2(str);
    /**
     * T=char const*
     * tmprv=char const*
     */

    func3(str);
    /**
     * T=char const [9]
     * tmprv=char const (&) [9]
     */


    func(str);
    /**
     * T=char const (&) [9]
     * tmprv=char const (&) [9]
     */
}
```

可以看到，这里是对数组的引用，这里数组的类型是`char const [9]`

#### 数组类型，套取长度

```cxx
template <typename T, unsigned L1>
void func4(T (&tmprv)[L1])
{
    cout << typeid(T).name() << endl;
    cout << typeid(tmprv).name() << endl;
    cout << "L1=" << L1 << endl;
}

int main(void) {
    func4(str);
    /**
     * c
     * A9_c
     * L1=9
     */
}
```

下面这是 C++ insight 的结果

```cxx
template <>
void func4<const char, 9>(const char (&tmprv)[9])
{
    std::operator<<(std::cout, typeid(char).name()).operator<<(std::endl);
    std::operator<<(std::cout, typeid(tmprv).name()).operator<<(std::endl);
    std::operator<<(std::cout, "L1=").operator<<(9U).operator<<(std::endl);
}
```

可以看到是保留 const 的，毕竟形参是`T&`

### 函数名做实参

#### 传值方式

```cxx
template <typename T>
void func2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

void test_func(int, int) { }

int main(void) {
    func2(test_func);
    /**
     * T=void (*)(int, int)
     * tmprv=void (*)(int, int) 类型：函数指针
     */
}
```

#### 传引用

```cxx
template <typename T>
void func3(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

void test_func(int, int) { }

int main(void) {
    func3(test_func);
    /**
     * T=void (int, int)
     * tmprv=void (&)(int, int) 类型：函数的引用
     */
}
```

#### 万能引用（与 传引用相同）

```cxx
template <typename T>
void func(T&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

void test_func(int, int) { }

int main(void) {
    func(test_func);
    /**
     * T=void (int, int)
     * tmprv=void (&)(int, int) 类型：函数的引用
     */
}
```

### 初始化列表做实参

#### 传值、传引用、万能引用

都不能编译通过：

```cxx
template <typename T>
void func(T&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void) {
    // func({ 1, 3 }); /* 错误：No matching function for call to 'func' */
}
```

#### 接受参数：初始化列表类型

```cxx
template <typename T>
void func5(std::initializer_list<T> tmprv)
{
    cout << "T=" << typeid(T).name() << endl;
    cout << "tmprv=" << typeid(tmprv).name() << endl;
}

int main(void) {
    func5({ 12, 3 });
    /**
     * T=i
     * tmprv=St16initializer_listIiE
     */
}
```

### 总结：

1. 推断中，引用类型的引用类型不存在（没有二级引用，只保留一次引用）
2. 万能引用，实参为左值或右值推断的结果不同
3. 按值传递的实参，传递给形参时 const 属性不起作用，但是传递进去 指针/数组 则另当别论
4. 数组 或者 函数类型在`func(T v)`中会被当做指针，但如果是`func(T& v)`则另当别论
5. 初始化列表不能直接推断成 initializer_list 类型，必须在函数模板的形参中明确指出使用 initializer_list 类型

## 3 - auto 类型常规推断

C++98，甚至是 C 语言中就有 auto 了，表示非 static 变量

但是在 C++11 以后，就有了新的含义了：用于变量的自动类型推断。
声明变量时候根据变量初始化的类型自动为此变量选择 匹配的类型，不需要程序员显式的定义类型

auto 的特点：

1. auto 的自动类型推断发生在编译期间
2. auto 定义变量必须立即进行初始化，这样编译器才能推断出 auto 类型 以及 这个变量类型（这两个不一定是同一个类型，看上面形参 和 T）。
   然后编译期间，就可以用真正的类型 替换掉 auto 这个类型占位符了（实际上编译器还是要得到准确的类型）
3. auto 的使用比较灵活，可以和指针、引用、const 等限定符结合使用

auto 推断后会代表一个具体类型，auto 就可以理解成类型模板中的 参数 T，然后变量的类型就像是上面的`tmprv`。

看上面的类型推断，其实我们也可以将 auto 分为三类：

1. 传值
2. 传引用、指针
3. 万能引用

### 传值方式（非指针，非引用）

```cxx
int main(void)
{
    auto x = 27; /* int */
    const auto x2 = x;
    const auto& xy = x;
    auto xy2 = xy; /* 这个相当于是 传值方式：引用--；const-- */
}
```

clangd 很好用 😋

### 指针或者引用类型，非万能引用

```cxx
int main(void)
{
    auto x = 27; /* int */
    const auto x2 = x;
    const auto& xy = x;
    auto xy2 = xy; /* 这个相当于是 传值方式：引用--；const-- */

    auto& xy3 = xy; /* const int & ，多余的 & 会被丢弃 */
    auto y = new auto(100); /* int *，形参是指针 */
    auto* xp2 = &x; /* int *；auto = int */
    auto xp3 = &x; /* int *；auto = int * */
}
```

### 万能引用类型

```cxx
int main(void) {
    auto&& wnyy0 = 222; /* int && */
    auto&& wnyy1 = x; /* int & */
    auto&& wnyy2 = x2; /* const int & */
}
```
