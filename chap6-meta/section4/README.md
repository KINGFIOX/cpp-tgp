# chap6_4 - 元组（tuple）实现

## 1 - 重要基础知识回顾

### 左值、右值、左值引用、右值引用

- 左值引用只能绑定到左值上。但是编译器，只会给 const 引用开绿灯，让他可以绑定左值，也可以绑定右值。
- 右值引用只能绑定到右值上

`int m`作为函数形参的时候，既可以接受：左值，也可以接受右值

这个 const 的左值引用 字面量，别动歪脑筋

```cxx
void mf(int&)
{
    std::cout << "call int&" << std::endl;
}

void mf(int&&)
{
    std::cout << "call int&&" << std::endl;
}

int main(void) {
    int& j = i;
    const int& k = 10;
    // int* p = std::remove_const<int*>(std::move(k));
    // 还是不行的，因为引用折叠，不能将 const int * 转换为 int *

    int&& a = 10;
    int&& b = a; /* 错误： Rvalue reference to type 'int' cannot bind to lvalue of type 'int' */

    mf(a); // call int&
    mf(std::forward<int>(a)); // call int&&
    mf(std::forward<int&&>(a)); // call int&&
    mf(std::forward<int>(b)); // call int&&
}
```

a 是一个 左值（右值引用类型）。左值：能够放到等号左侧。
这个传参，隐藏着一个：`形参 = 实参`的操作。

`int && = a`不行，因为 a 是左值，没法绑定到 右值引用。`int& = a`就可以。

### std::move 究竟干了什么

`std::move`的结果是一个：右值引用类型（本质上还是 左值）。

`std::move`将左值转换为了 右值引用类型

### std::forward 究竟干了什么

forward ：

- 左值引用：左值`int&`
- 右值引用：右值`int&&`
- 既不是左值引用，也不是右值引用：右值`int`

```cxx
int main(void)
{
    // int i = 10;
    // int&& j = i;  // 右值引用 只能绑定 右值
    int i = 10;
    int&& j = std::forward<int>(i);
    j = 50;
    std::cout << i << std::endl; // 50
}
```

`std::forward`的两个作用：

1. 提高程序运行效率
2. 用于完美转发

### 万能引用（又被称为 转发引用）

万能引用是一种类型

```cxx
template <typename T>
void myfunc(T&& tmprv) { }

class A {
public:
    template <typename U> // tmprv 万能引用类型
    void myfunc1(U&& tmprc) { }

    void myfunc2(U&& tmprc) { } // tmprv 不是万能引用类型
};
```

此时这个`tmprv`的类型就是万能引用类型

### 完美转发

中间函数当跳板，看 chap3_3

`std::forward`将：右值引用/非引用 拧成了 右值

## 2 - 元组基本概念、基础代码的设计与实现

### 泛化、特化、构造函数的实现

### 拷贝构造函数 及 拷贝构造模板 的实现

### 实现获取 tuple 中的 get 接口

### std::make_tuple 的实现

### 总结

## 3 - 操作接口（算法）
