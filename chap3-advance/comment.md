# chap3 - 模板进阶知识

## xmake + boost

我的评价是：xmake 直接一键解决

## decltype

`decltype` 是一个 C++11 引入的关键字，用于查询表达式的类型。编译器分析表达式并得出它的类型，`decltype` 不会实际计算表达式的值。它非常有用，尤其是在模板编程和类型推导的上下文中，因为它允许编写者获得表达式的准确类型。

以下是一些 `decltype` 的用法示例：

```cpp
int x = 1;
decltype(x) y = 2; // y 的类型是 int，因为 x 的类型是 int

const int& z = x;
decltype(z) w = y; // w 的类型是 const int&，因为 z 的类型是 const int&

auto f = [](int a) -> int { return a; };
decltype(f) g = f; // g 的类型是与 lambda 表达式相同的类型

struct S { double x; };
const S* s = new S();

decltype(s->x) u; // u 的类型是 double
decltype((s->x)) v = u; // v 的类型是 const double&，注意双括号

int arr[5];
decltype(arr) arr2; // arr2 的类型是 int[5], 与 arr 类型相同

int n;
decltype(n = 3) n2 = n; // n2 的类型是 int&
```

在上面的例子中，可以看到 `decltype` 能够处理不同类型的场景，包括变量、引用、函数、成员变量、数组等。

`decltype` 在模板编程中尤其有用，因为可以根据模板参数推导出返回类型或变量类型，例如：

```cpp
template<typename X, typename Y>
auto add(X x, Y y) -> decltype(x + y) {
    return x + y;
}
```

在以上模板函数中，`decltype(x + y)` 用于推导 `add` 函数返回类型，这取决于 `x` 和 `y` 的类型及其相加的结果类型。

`decltype` 还被广泛用于 C++11 及以后版本的类型推导和自动类型推导中，尤其是与 `auto` 关键字搭配使用。

## std::ref 的实现

`std::ref` 是 C++11 标准库中的一个函数模板，它用于创建一个对象的引用包装器（`std::reference_wrapper` 类的实例）。
这允许将引用作为值传递，例如在需要以值传递参数的函数（如线程构造函数或`std::bind`）中传递引用。

实现一个简单版本的 `std::ref` 可以帮助理解其工作原理，但请注意，下面的简化实现不包括标准库中的所有功能和安全检查。

```cpp
#include <iostream>
#include <functional> // For std::reference_wrapper

// A simplified version of std::ref
template <typename T>
std::reference_wrapper<T> SimpleRef(T& t) {
    return std::reference_wrapper<T>(t);
}

int main() {
    int x = 10;
    auto refX = SimpleRef(x);

    // refX is a reference_wrapper, you can access the original variable using .get()
    std::cout << "refX holds: " << refX.get() << std::endl;

    // You can also assign a new value to the referenced variable
    refX.get() = 20;
    std::cout << "x is now: " << x << std::endl; // x will be 20

    return 0;
}
```

在上面的代码中，`SimpleRef` 函数模板接收一个引用参数，并返回一个 `std::reference_wrapper` 对象。
然后，您可以使用 `reference_wrapper` 的 `get` 方法来访问被包装的引用。

如果您想要自己实现 `std::reference_wrapper` 的功能，而不是使用标准库中的版本，您可以尝试下面的实现：

```cpp
#include <iostream>
#include <functional> // For std::reference_wrapper

// A very simplified implementation of reference_wrapper
template <class T>
class MyReferenceWrapper {
public:
    // Constructor takes a reference and stores its address
    MyReferenceWrapper(T& ref) : ptr(&ref) {}

    // Overload operator() to return the reference
    operator T& () const { return *ptr; }

    // Function to get the reference
    T& get() const { return *ptr; }

private:
    T* ptr; // Store the address of the referenced object
};

// A simplified version of std::ref
template <typename T>
MyReferenceWrapper<T> MyRef(T& t) {
    return MyReferenceWrapper<T>(t);
}

int main() {
    int x = 10;
    auto refX = MyRef(x);

    // refX is a MyReferenceWrapper, you can access the original variable using .get()
    std::cout << "refX holds: " << refX.get() << std::endl;

    // You can also assign a new value to the referenced variable
    refX.get() = 20;
    std::cout << "x is now: " << x << std::endl; // x will be 20

    return 0;
}
```

这个实现提供了一个非常基础的 `MyReferenceWrapper` 类，它模拟了 `std::reference_wrapper` 的基本行为。
实际上，标准库中的 `std::reference_wrapper` 更复杂，提供了更多的功能和安全性检查。
在生产代码中，应该始终使用标准库提供的 `std::ref` 和 `std::reference_wrapper`。
