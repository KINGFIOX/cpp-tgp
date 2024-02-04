# chap3_1 - 万能引用 universal/forwarding reference

## 1 - 类型区别基本含义

## 2 - universla reference、万能引用、未定义引用 的基本认识

结论：万能引用是一种类型，就跟 int 是一种类型是一个道理。

右值引用：右值引用类型，是用&&符号表示。右值引用是绑定到一个右值上。
比方说`int&& rv = 1000;`右值引用，绑定到右值上

### 右值引用

```cxx
void func(int&& tmprv) /* 参数tmprv是一个右值引用 */
{
    printf("%d", tmprv);
}

int main(void)
{
    func(10);

    int&& rv = 1000;
    printf("%p\n", &rv); // 0x16d8aecbc
    rv = 10;
    printf("%p\n", &rv); // 0x16d8aecbc

    func(100);
    int i = 100;
    // func(i); /* 错误： no matching function for call to 'func' */
}
```

改成模板函数，`func(i)`不报错了

```cxx
template <typename T>
void func(T&& tmprv)
{
    cout << tmprv << endl;
    return;
}

int main(void)
{
    func(100);
    int i = 100;
    func(i);
}
```

说明一件事情，当`func(i)`的时候，这个 T 并不代表`int`类型。

几条事实：

1. 函数模板中的`template <typename T> void func(T&& tmprv)`给他左值、右值都是可以的
2. `tmprv`的类型是`T&&`
3. 只有在类型推断的时候，才会出现：既能够接受左值，又能够接受右值

万能引用离不开两种语境：

1. 必须是函数模板（注意是 函数）
2. 发生了类型推断，而且形参的类型是`T&&`（这就是万能引用，万能引用是一种类型）

我们可以发现，万能引用 与 右值引用长得比较像

万能引用作为：函数形参类型的时候，既可以接受左值，也可以接受右值
（或者也可以叫：未定义引用，这里不会要求是左值、也不会要求是右值）

如果实参传递了一个 整型左值：tmprv 的类型最后会被推断为`int &`类型；
如果实参传递了一个 整型右值：tmprv 的类型最后会被推断为`int &&`类型

### 判断题

1. `void func(int&& param) { ... }`，右值引用
2. `template<typename T> void func(T && tmpvalue) { ... }`，万能引用
3. `void func(std::vector<T>&& param) { ... }`，这只是右值引用，并不是万能引用

什么情形才是万能引用？

1. 一个是函数模板中用作函数参数的类型推断（参数中要设计类型推断），`T&&`
2. `auto &&tmpvalue = ...` 这个也是万能引用，后续再说

其他情况的`&&`都是右值引用

```cxx
template <typename T>
void func(T&& tmprv)
{
	tmprv = 12;
    cout << tmprv << endl;
    return;
}
```

这里，不管`tmprv`的类型是左值引用还是右值引用，tmprv 都是左值，
既然 tmprv 是左值，那么就是可以给 tmprv 进行赋值

## 3 - 万能引用资格的剥夺与辨认

### 剥夺

const 会剥夺万能引用的资格，被打回原型成右值引用

```cxx
template <typename T>
void func(const T&& tmprv)
/**
 * 1. 有 const 修饰，就不再是万能引用了（万能引用的资格被剥夺，退化了）
 * 2. 又因为有 &&，因此这不过是一个右值引用
 */
{
}

int main(void)
{
    func(100);
    int i = 100;
    func(i); /* 错误： no matching function for call to 'func' */
}
```

### 辨认（两小儿辩日）

```cxx
template <typename T>
class myTestC {
public:
    /* 这个不是万能引用，因为不能类型推断 */
    void testfunc(T&& x) { }
};

int main(void)
{
    myTestC<int> mc;
    int i = 100;
    mc.testfunc(i); /* 错误： Rvalue reference to type 'int' cannot bind to lvalue of type 'int' */
    mc.testfunc(std::move(i)); /* ✅ */
}
```

为什么这个是错误的，因为没有涉及到类型推断，看上面《语境》

那么如何实现：成员函数是万能引用呢？

```cxx
template <typename T>
class myTestC {
public:
    /* 这个成员函数的 x 形参就是：万能引用类型 */
    template <typename T2>
    void testfunc2(T2&& x) { }
};

int main(void)
{
    myTestC<int> mc;
    mc.testfunc2(i); /* ✅ */
    mc.testfunc2(100); /* ✅ */
}
```
