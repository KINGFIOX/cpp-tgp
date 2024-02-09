# chap5 -

## 零初始化

在 C++中，零初始化（zero-initialization）是指将一个对象的存储区域全部设为零。可以进行零初始化的类通常需要满足以下条件：

1. **非引用成员**：类中不能有引用类型的成员，因为引用必须绑定到一个对象上，而不能是零。

2. **无用户定义的构造函数**：如果类中定义了自己的构造函数，那么它将不会是零初始化，而是会调用相应的构造函数进行初始化。

3. **无虚函数和虚基类**：类中如果有虚函数或者从虚基类继承而来，那么对象的初始化将包含设置虚函数表指针（vptr）的步骤，因此不会是纯粹的零初始化。

4. **非类类型成员的默认初始化为 0**：如果类中包含的其他非类类型的成员变量（如内置类型的变量）在默认初始化时其值为 0，则这些成员可以进行零初始化。

零初始化可以在以下几种情况下发生：

- 对静态存储期的对象如果没有其他初始化，则会进行零初始化。
- 当创建对象时，使用`()`或`{}`没有提供初始值时，如果对象是内置类型，它会被零初始化。
- 在`new`表达式中，如果使用`()`没有提供初始值，则对象会被零初始化。
- 对于局部作用域的对象，如果使用`()`没有提供初始值，且对象是非 POD（Plain Old Data）类型，则它会被零初始化。

举例说明，以下是可以进行零初始化的类：

```cpp
class CanZeroInitialize {
public:
    int a;
    double b;
    char c[10];
    // 没有用户定义的构造函数
    // 没有虚函数或虚基类
    // 没有引用成员
};

// 零初始化示例
CanZeroInitialize obj1{}; // 所有成员都设为0
CanZeroInitialize* obj2 = new CanZeroInitialize(); // 所有成员都设为0
static CanZeroInitialize obj3; // 静态存储期对象，所有成员都设为0
```

在上述例子中，`obj1`、`obj2`和`obj3`都会进行零初始化。如果`CanZeroInitialize`类中包含用户定义的构造函数、虚函数或者引用成员，那么就不能仅仅使用花括号或小括号来实现零初始化了。