# chap5_1 - 萃取技术 及 fixed traits（固定萃取）演示

traits（萃取）技术用于对模板中的各种模板参数进行管理。
traits 是 特征的意思，但是我们这里翻译成了 萃取

type traits: 类型萃取，实现了许多的类型信息的提取

学习目的：

1. 了解标准库中许多 trait 技术的实现方法
2. 灵活运用并结合这些实现方法，写出功能 更强大、更优雅和使用的代码

fixed traits：给进来一种类型，萃取出另一种类型

## 1 - 一个常规范例

```cxx
/**
 * @brief 计算和值的 函数模板
 */
template <typename U, typename T>
U funcSum(const T* begin, const T* end)
{
    U sum {};
    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    int arr1[] = { 10, 15, 20 };
    int arr2[] = { 1000000000, 1500000000, 2000000000 };
    char arr3[] = "abc";

    cout << funcSum<int64_t>(&arr1[0], &arr1[2]) << endl;
    cout << funcSum<int64_t>(&arr2[0], &arr2[2]) << endl;
    cout << funcSum<int64_t>(&arr3[0], &arr3[2]) << endl;
}
```

这里不方便的地方就是：需要手动指定`int64_t`，那有没有更好的解决方案呢？
采用 固定萃取：看 `s5_1/p2.cxx`

## 2 - 迭代起范例

5 类迭代起：

1. 输出型迭代器：`struct output_iterator_tag`
2. 输入型迭代器：`struct input_iterator_tag`
3. 前向迭代器：`struct forward_iterator_tag`
4. 双向迭代器：`struct bidirectional_iterator_tag`
5. 随机访问迭代器：`struct random_access_iterator_tag`

- vector 容器：随机访问迭代器
- list 容器：双向迭代器

本节帮助大家了解：如何通过 固定萃取技术 从容器中萃取出对应的迭代器种类

list 容器对应的迭代器，是一个类或者类模板

```cxx
class _List_iterator{
public:
    using iterator_category = bidirectional_iterator_tag; // 代表 list 容器的迭代器属于哪一个种类的 一个别名类型
    // ...
};

template <class T>
class list{
public:
    using iterator = _List_iterator;
};
```

不难想象：

`list<int>::iterator::iterator_category == bidirectional_iterator_tag`，
这里的`list<int>::iterator`就是`_List_iterator`

1. 在容器中，标示出：所属迭代器的种类
2. fixed trait 类模板，是通过 iterator_traits 实现的

```cxx
void _display_category(std::random_access_iterator_tag mytag)
{
    std::cout << "random_access_iterator_tag" << std::endl;
}

template <typename IterT>
struct IteratorTraits {
    using iterator_category = typename IterT::iterator_category;
};

template <typename T>
void display_category(T iter)
{
    // typename std::iterator_traits<T>::iterator_category cagy; // 萃取机，cagy 是对象
    typename IteratorTraits<T>::iterator_category cagy; // 萃取机，cagy 是对象
    _display_category(cagy); /*  */
}

int main(void)
{
    display_category(std::vector<int>::iterator()); // 创建了一个临时的 _Vector_iterator 类对象，
    /* random_access_iterator_tag */
}
```

1. `display_category(vector<int>::iterator())` 创建了一个临时的 `_Vector_iterator` 类对象
2. `display_category`中的代码行`typename IteratorTraits<T>::iterator_category cagy;`等价于
   `typename IteratorTraits<_Vector_iterator>::iterator_category cagy;`
3. iterator_traits 类模板定义中 `IteratorTraits<T>::iterator_category = T::iterator_category`；
   代码行等价于`_Vector_iterator::iterator_category`
4. `_Vector_iterator::iterator_category = random_access_iterator_tag`。通过这个类型创建了`cagy`对象
5. 这个对象传入到对应的`_display_category`，有种多态的感觉

## 3 - 通过容器（数组）类型萃取元素类型范例

### 用 GetEleType 类模板进行常规实现

```cxx
/**
 * @brief 泛化版本不实现
 *
 * @tparam T
 */
template <typename T>
struct GetEleType;

template <typename T>
struct GetEleType<std::vector<T>> {
    using type = T;
};

/**
 * @brief 数组
 * （这个依然是：参数范围上的 偏特化）
 * @tparam T
 * @tparam Size 看起来这个特化版本多了一个 模板参数（但并非是 参数数量上的偏特化）
 */
template <typename T, std::size_t Size>
struct GetEleType<T[Size]> {
    using type = T;
    static const std::size_t size = Size;
};
```

偏特化怎么填写？就是把泛化版本规定的参数给填写完成`template <typename T> struct GetEleType;`。
先确定如何填写 泛化版本，再反推泛化版本的 参数有哪些

## 4 - 引用类型的移除和增加

萃取的意思就是：给进去一个类型，返回出了另一个类型

### 移除

C++标准库中提供了 `std::remove_reference`类模板

```cxx
template <class T1, class T2>
void print_is_same()
{
    std::cout << typeid(T1).name() << std::endl;
    std::cout << typeid(T2).name() << std::endl;
    std::cout << "T1 == T2: " << std::is_same<T1, T2>() << std::endl; // () 产生了临时对象，并转换为 int
    /* 类型转换运算符 constexpr opeator value_type() const noexcept {}; */
    // std::cout << "T1 == T2" << std::is_same<T1, T2>::value << std::endl; 这两个写法一样的
}

int main(void)
{
    std::remove_reference<int>::type a;
    std::remove_reference<int&>::type b;
    std::remove_reference<int&&>::type c;

    print_is_same<decltype(a), decltype(b)>();
    print_is_same<decltype(a), decltype(c)>();
}
```

手动实现 remove_reference

```cxx
template <typename T>
struct RemoveReference {
    using type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

int main(void)
{
    using boost::typeindex::type_id_with_cvr;
    int a1 = 12;
    int& a2 = a1;
    int&& a3 = 12;
    std::cout << type_id_with_cvr<decltype(a3)>().pretty_name() << std::endl; // int (*&&)(int, int)
    std::cout << type_id_with_cvr<RemoveReference_t<decltype(a3)>>().pretty_name() << std::endl; // int (*&&)(int, int)
}
```

### 增加

根据给定的类型创建：左值引用 或者是 右值引用

1. `std::add_lvalue_reference`类模板：给进来 一个类型，返回该类型对应的左值引用类型
2. `std::add_rvalue_reference`类模板：给进来 一个类型，返回该类型对应的右值引用类型
3. `std::is_lvalue_reference`和`std::is_rvalue_reference`类模板，判断某个类型是否是 左值引用类型、右值引用类型

```cxx
template <typename T>
struct AddLValueReference {
    using type = T&;
};

template <typename T>
struct AddRValueReference {
    using type = T&&;
};

int main(void)
{
    int a = 15;
    using boost::typeindex::type_id_with_cvr;
    typename AddLValueReference<decltype(a)>::type b = a;
    typename AddRValueReference<decltype(a)>::type c = 16;
    std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(b)>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(c)>().pretty_name() << std::endl;
}
```

注意：`add_rvalue_reference<int&>::type`是`int& && ==> int&`（引用折叠）

## 5 - const 修饰符的移除

```cxx
/**
 * @brief 泛化版本
 *
 */
template <typename T>
struct RemoveConst {
    using type = T;
};

/**
 * @brief 特化版本
 *
 */
template <typename T>
struct RemoveConst<const T> {
    using type = T;
};

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

int main(void)
{
    RemoveConst_t<const int> nca = 15;
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<decltype(nca)>().pretty_name() << std::endl;
}
```

## 6 - 退化（decay）技术

某些类型，一旦调用了函数模板（自动推断），然后推断出来的一些类型，会发现 退化(decay) 现象。
退化的意思就是：把类型的某一些修饰符给丢弃了。

退化：对于 const int 类型来说，int 类型就是一种退化的表现

```cxx
template <typename T>
void func(T tmpRv)
{
    std::cout << "---------- begin ----------" << std::endl;
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(tmpRv)>().pretty_name() << std::endl;
    std::cout << "---------- end ----------" << std::endl;
}

void testFunc()
{
}

int main(void)
{
    const int a = 16;
    const int& b = a;
    const int&& c = 18;

    int arr[5] = { 1, 2, 3, 4, 5 };

    func(a);
    /**
     * int
     * int
     */
    func(b);
    /**
     * int
     * int
     */
    func(c);
    /**
     * int
     * int
     */
    func(arr);
    /**
     * int*
     * int*
     */
    func(testFunc);
    /**
     * void (*)()
     * void (*)()
     */
}
```

因为 形参是：传值，因此 const 和 `&`、`&&` 全部被丢掉了。

1. 修饰符丢弃
2. 数组 ---> 指针
3. 函数名 ---> 函数指针

这些都是 类型上的退化表现（decay）

C++ 标准库：`std::decay`，该类模板的作用就是：把 类型 退化掉

```cxx
int main(void) {
    std::decay<const int&>::type nb = 26; /* nb 类型为：int */
}
```

函数类型的基本概念：

1. 函数类型：由函数返回值、函数参数决定
2. 可以利用函数指针，指向某种函数类型，若要指向`void func2()`，那么函数指针类型是`void(*)()`
   定义函数指针：`void (*p)() = func2;`
3. 必须要为：函数名退化成函数指针 特化。否则会出现一些问题：

```cxx
void f2()
{
    std::cout << "f2 执行了" << std::endl;
}

int main(void) {
    Decay<decltype(testFunc2)>::type f2; // void () 表面看起来此代码行定义了一个 函数类型的变量 f2；
                                         // 实际理解成函数声明更好。类似于 void f2();
                                         // 如果我有一个 已经存在的同名 f2
    std::cout << type_id_with_cvr<decltype(f2)>().pretty_name() << std::endl;
    f2(); // 调用 f2() 函数
}
```

### 手动实现 decay

```cxx
/* ---------- 删除 引用 ---------- */

template <typename T>
struct RemoveReference {
    using type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

/* ---------- 删除 const ---------- */

/**
 * @brief 泛化版本
 *
 */
template <typename T>
struct RemoveConst {
    using type = T;
};

/**
 * @brief 特化版本
 *
 */
template <typename T>
struct RemoveConst<const T> {
    using type = T;
};

/* ---------- remove CR ---------- */

/**
 * @brief 把 const 和 引用 去掉
 *
 * @tparam T
 */
template <typename T>
struct RemoveCR : RemoveConst<typename RemoveReference<T>::type> { };
// 1. RemoveReference<T>::type 去掉 引用 的类型
// 2. RemoveConst<typename RemoveReference<T>::type> 中有成员 type
// 3. RemoveCR 继承 RemoveConst<typename RemoveReference<T>::type>，相当于是继承了成员 type
// 个人想法：也可以 using type = RemoveConst<typename RemoveReference<T>::type>::type 实现

template <typename T>
using RemoveCR_t = typename RemoveCR<T>::type;

template <typename T>
struct RMCR {
    using type = RemoveConst<typename RemoveReference<T>::type>::type;
};

template <typename T>
using RMCR_t = typename RMCR<T>::type;

/* ---------- decay ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam T
 */
template <typename T>
struct Decay : RMCR<T> { };

/**
 * @brief 针对 (有边界)数组 的特化版本，这不继承 RMCR
 *
 * @tparam T
 * @tparam Size
 */
template <typename T, std::size_t Size>
struct Decay<T[Size]> {
    using type = T*;
};

/**
 * @brief 针对 (无边界)数组 的特化版本，这不继承 RMCR
 *
 * @tparam T
 */
template <typename T>
struct Decay<T[]> {
    using type = T*;
};

template <typename T, typename... Args>
struct Decay<T(Args...)> {
    using type = T (*)(Args...);
};

template <typename T>
using Decay_t = Decay<T>::type;

/* ----------  ---------- */

void testFunc2()
{
    std::cout << "testFunc2 执行了" << std::endl;
}

void f2()
{
    std::cout << "f2 执行了" << std::endl;
}

int main(void)
{
    /* 1. 去掉修饰符 */
    std::decay<const int&>::type nb1 = 26; /* nb1 类型为：int */
    std::cout << type_id_with_cvr<decltype(nb1)>().pretty_name() << std::endl;

    // /* 2. 数组 */
    // std::decay<int[10]>::type nb2; /* nb2 类型为：int* */
    // std::cout << type_id_with_cvr<decltype(nb2)>().pretty_name() << std::endl;

    /* 2. 数组 */
    Decay<int[10]>::type nb2; /* nb2 类型为：int* */
    std::cout << type_id_with_cvr<decltype(nb2)>().pretty_name() << std::endl;

    /* 测试 */
    RMCR<const int&>::type nb3 = 26;
    std::cout << type_id_with_cvr<decltype(nb3)>().pretty_name() << std::endl;

    Decay<decltype(testFunc2)>::type f2;
    std::cout << type_id_with_cvr<decltype(f2)>().pretty_name() << std::endl; // void(*)()
    // f2(); // 错误：execv(./chap5-traits/section1/s1_5/build/p4 ) failed(-1)
    f2 = testFunc2;
    f2();
}
```
