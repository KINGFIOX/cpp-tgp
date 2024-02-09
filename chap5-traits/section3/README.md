# chap5_3 - 使用了 SFINAE 特性的信息萃取

SFINAE: substitude fail is not a error

## 1 - 用成员函数重载实现 is_default_constructible

`is_default_constructible`类模板：判断一个类对象是否能够被默认构造

```cxx
class A { };

class B {
public:
    B(int tmpVal) { }
};

int main(void)
{
    std::cout << std::is_default_constructible<int>::value << std::endl; // 1
    std::cout << std::is_default_constructible<A>::value << std::endl; // 1
    std::cout << std::is_default_constructible<B>::value << std::endl; // 0
}
```

下面是一种实现方法：

```cxx
template <typename T>
class IsDefaultConstructible {
private:
    /* 函数模板，只有声明，没有实现 */
    template <typename = decltype(T())>
    static std::true_type test(void*);

    /* 重载版本，这个 test 接受 可变形参，这个 typename = int 的 int 是随便给的 */
    /* 编译器会 优先选择：有具体形参的版本，带 ... 形参的 test 具有最低被匹配的优先级 */
    template <typename = int>
    static std::false_type test(...);

public:
    static constexpr bool value = IsSameType<decltype(test(nullptr)), std::true_type>::value;
};

// 如果 T 支持默认构造。那么 value = IsSameType<decltype(test(nullptr), std::true_type)>::value
// decltype(test(nullptr)) 推断 test(nullptr) 的返回类型
// test(nullptr) 优先选择 template<typename = decltype(T())> static std::true_type test(void*);
// 如果 T() 能够被默认构造出来，就没啥问题，那么 test(nullptr) 就是返回 std::true_type，
// 否则就是返回 std::false_type

// 核心特点：使用了同名，但是不同返回类型的：成员函数模板

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

class B {
public:
    B(int tmpVal) { }
};

int main(void)
{
    std::cout << IsDefaultConstructible<int>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<A>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<B>::value << std::endl; // 0
}
```

但是不知道为什么，我的 llvm - clang 就没法搞那个 B，难道是不同的编译器，对 SFINAE 有不一样的规则嘛，不清楚

## 2 - 用成员函数重载实现 is_convertible

`std::is_convertible`类模板能力是：用于判断：能否从某个类型隐式转换为另一个类型

```cxx
class A { };
class B : public A { };

int main(void) {
    std::cout << std::is_convertible<A, B>::value << std::endl; // 0 ： A ---> B 是不行了
    std::cout << std::is_convertible<B, A>::value << std::endl; // 1
}
```

下面是 `is_convertible` 的实现

```cxx
template <typename FROM, typename TO>
struct IsConvertibleHelper {
private:
    static void testfunc(TO)
    {
    }

    /* 这里发生了 隐式类型转换；实参是 from，形参是 */
    template <typename = decltype(testfunc(std::declval<FROM>()))>
    /* 错误： No viable conversion from 'A' to 'B' */
    static std::true_type test(void*);

    static std::false_type test(...);

public:
    using type = decltype(test(nullptr));
};

template <typename FROM, typename TO>
struct IsConvertible : public IsConvertibleHelper<FROM, TO>::type { };

/* ---------- ---------- 测试代码 ---------- ----------  */

class A { };

class B : public A { };

int main(void)
{
    std::cout << IsConvertible<A, B>::value << std::endl;
    std::cout << IsConvertible<B, A>::value << std::endl;
}
```

## 3 - 用成员函数重载实现 is_class

用于判断：是否是一个类类型（不是联合类型）的类模板
下面是实现代码：

```cxx
/* ---------- ---------- is_same_type ---------- ---------- */

template <typename T, typename U>
struct IsSameType : std::false_type { };

template <typename T>
struct IsSameType<T, T> : std::true_type { };

/* ---------- ---------- is_class ---------- ---------- */

template <typename T>
class IsClass {
private:
    /* 这里的 U 实际上就是 T，看我们调用的时候传入的参数。这里是为了：类型推断 */
    /* int U::* 表示：成员变量指针，省略掉了 指针名 */
    /* 只有：类类型，或者是 union 类型，才能匹配到这个 test(int U::*)，否则 SFINAE */
    template <typename U>
    static std::integral_constant<bool, !std::is_union<U>::value> test(int U::*);

    template <typename>
    static std::integral_constant<bool, false> test(...);

public:
    static constexpr bool value = IsSameType<
        decltype(test<T>(nullptr)),
        std::integral_constant<bool, true>>::value;
};

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

union B {
};

int main(void)
{
    std::cout << IsClass<A>::value << std::endl; // 1
    std::cout << IsClass<int>::value << std::endl; // 0
    std::cout << IsClass<B>::value << std::endl; // 0

    int A::*mp = nullptr; // 指向类 A 中的某一个成员变量，并且成员变量的类型是 int

    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<decltype(mp)>().pretty_name() << std::endl; // int A::*
}
```

## 4 - 用成员函数重载实现 is_base_of

用于判断：某一个类是否是另一个父类的子类。

注意一下与`is_convertible`的区别。

就是如果写了 类型转换函数，其实也有可能进行转换。但是不一定就是继承的关系

```cxx
class A { };

class B : public A {
};

class C : private A {
};

int main(void)
{
    std::cout << std::is_base_of<A, B>::value << std::endl; // 1
    std::cout << std::is_base_of<A, C>::value << std::endl; // 1
    // std::cout << std::is_convertible<C, A>::value << std::endl; // 0，组合关系，不是 is-a 不能转换
}
```

### 先看一下 继承关系的 函数调用

```cxx
class PAR { };

class SON : public PAR { };

class SON_o : private PAR { };

class OTHER { };

void f(PAR*)
{
    std::cout << "f(PAR*) 执行了" << std::endl;
}

void f(void*)
{
    std::cout << "f(void*) 执行了" << std::endl;
}

int main(void)
{
    f(static_cast<PAR*>(nullptr)); /* f(PAR*) */
    f(static_cast<SON*>(nullptr)); /* f(PAR*) */
    // f(static_cast<SON_o*>(nullptr)); /* 错误： Cannot cast 'SON_o' to its private base class 'PAR' */
    f(static_cast<OTHER*>(nullptr)); /* f(void*) */
}
```

但是其实注意一下，这其实原理是发生了隐式类型转换，所以这个我不好评价。
因此，下面这个老师实现的版本，个人感觉并不是真正的 `is_base_of`。

```cxx
template <typename T, typename U>
struct IsSameType : std::false_type { };

template <typename T>
struct IsSameType<T, T> : std::true_type { };

template <typename Base, typename Derived>
class IsBaseOf {
private:
    template <typename T>
    static std::true_type test(T*);

    template <typename>
    static std::false_type test(void*);

    template <typename B, typename D>
    static auto test_middle() -> decltype(test<B>(static_cast<D*>(nullptr)));
    // 返回类型后置语法
    // static_cast<D*>(nullptr) 得到了一个 D* 类型的指针
    // 调用 std::true_type test(Base*) 或者 std::false_type test(Base*) 函数

public:
    static constexpr bool value = IsSameType<
        std::integral_constant<bool,
            std::is_class<Base>::value &&
            std::is_class<Derived>::value &&
            decltype(test_middle<Base, Derived>())::value>,
        /* 但是如果这里出现 C 私有继承 A，这是有问题的 */
        std::integral_constant<bool, true>
    >::value;
};
```

## 5 - 用类模板重载实现 is_default_constructible

上面我们是通过：成员函数实现的：`is_default_constructible`。
这里，我们要使用`void_t`别名模板

```cxx
template <typename ... Args>
using void_t = void;
```

`void_t`用途：能够检测到：引用 SFINAE 特性是出现的非法类型

```cxx
/* ---------- ---------- is Default constructible ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T, typename U = std::void_t<>>
class IsDefaultConstructible : public std::false_type { };

/**
 * @brief 特化版本
 *
 * @tparam T
 * @tparam U
 */
template <typename T>
class IsDefaultConstructible<T, std::void_t<decltype(T())>> : public std::true_type { };

/* ---------- ---------- 测试 ---------- ---------- */

class A { };

class B {
public:
    B(int tmpValue)
    {
    }
};

int main(void)
{
    std::cout << IsDefaultConstructible<A>::value << std::endl; // 1
    std::cout << IsDefaultConstructible<B>::value << std::endl; // 0
}
```

这个范例，与之前的：`is_copy_assignable`非常类似
