# comment - 基本

## decltype(auto)

`decltype(auto)` 是 C++11 引入的一种类型声明，它用于指示编译器推断变量或表达式的类型，
并保留该类型的引用和 cv 限定符（const 和 volatile）。
这主要用于简化代码，使得声明的类型与表达式的类型一致。

具体来说，`decltype(auto)` 的使用场景如下：

1. **自动类型推断：** 当你希望从一个表达式中推断出变量的类型，并且希望保留该表达式的引用和 cv 限定符时，可以使用 `decltype(auto)`。

   ```cpp
   int x = 42;
   const int& y = x;

   decltype(auto) a = x;  // a 的类型是 int，保留 x 的类型和 cv 限定符
   decltype(auto) b = y;  // b 的类型是 const int&，保留 y 的类型和 cv 限定符
   ```

2. **用于返回类型推断：** 在函数返回类型中使用 `decltype(auto)` 可以使得函数的返回类型与表达式的类型一致，同时也保留引用和 cv 限定符。

   ```cpp
   auto func1() -> decltype(auto) {
       int x = 42;
       return x;  // 返回类型是 int
   }

   auto func2() -> decltype(auto) {
       int y = 42;
       return y;  // 返回类型是 int&
   }
   ```

   在上述例子中，`func1` 返回的类型是 `int`，而 `func2` 返回的类型是 `int&`。

使用 `decltype(auto)` 时需要注意，它并不会进行类型的实际推导，而是完全依赖于表达式的类型。
如果表达式是一个引用，那么 `decltype(auto)` 也会得到引用类型。

## using

`using qualifier::name;` 和 `using namespace identifier;` 是 C++ 中两个不同的指令，它们的作用分别是引入单个名称和引入整个命名空间。

1. `using qualifier::name;`:

   - 这个语句用于引入一个特定的名称，使得你可以在代码中直接使用该名称，而不需要加上命名空间前缀。
   - 示例：
     ```cpp
     using std::cout; // 引入命名空间 std 中的 cout
     int main() {
         cout << "Hello, World!" << std::endl; // 不再需要使用 std:: 前缀
         return 0;
     }
     ```

2. `using namespace identifier;`:
   - 这个语句用于引入整个命名空间，使得该命名空间中的所有名称都在当前作用域可用。
   - 示例：
     ```cpp
     using namespace std; // 引入整个命名空间 std
     int main() {
         cout << "Hello, World!" << endl; // 不再需要使用 std:: 前缀
         return 0;
     }
     ```

区别：

- `using qualifier::name;` 只引入特定的名称，而 `using namespace identifier;` 引入整个命名空间的所有名称。
- 使用 `using namespace` 可能会导致命名冲突，因为它将整个命名空间的名称引入当前作用域，可能与其他命名空间或全局作用域中的名称发生冲突。因此，最好在全局范围内或者只在小范围内使用 `using namespace`，以减少潜在的命名冲突。

## 不能重定义缺省参数

```cxx
template <typename T, typename U, size_t arrsize = 8>
struct TC {
    T m_arr[arrsize]; /* 因为 arrsize 是：编译期间就确定的 */
    void functest();
};

/* 错误： arrsize: Cannot add a default template argument to the definition of a member of a class template */
template <typename T, typename U, size_t arrsize = 8>
void TC<T, U, arrsize>::functest() { }
```
