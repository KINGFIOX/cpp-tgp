# comment

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
