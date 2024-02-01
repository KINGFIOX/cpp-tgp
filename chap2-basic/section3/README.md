# chap2_3 - 类模板中的友元

## 友元类

让某个类 B 成为另外一个类 A 的友元类，这样的话，类 B 就可以在其成员函数中访问 A 的所有成员，
而不用管这些成员在类 A 中是用什么（public，protected，private）来修饰的

如果现在 类 A 和类 B 都编程了类模板，那么能否让类模板 B 称为类模板 A 的友元类模板呢？

### 让类模板的某个实例成为友元类

```cxx
template <typename U>
class B; /* 一定要前向声明 */

template <typename T>
class A {
public:
    friend class B<long>;

private:
    int data;
};

template <typename U>
class B {
public:
    void callBAF()
    {
        A<int> atmpObj;
        atmpObj.data = 5;
        cout << atmpObj.data << endl;
    }
};

int main(void)
{
    B<long> bl;
    bl.callBAF();

    // B<int> bi; // 编译错误
    // bi.callBAF();
}
```

### 让类模板成为友元类模板

```cxx
// template <typename U>
// class B;

template <typename T>
class A {
public:
    template <typename U>
    friend class B;

private:
    int data;
};

template <typename U>
class B {
public:
    void callBAF()
    {
        A<int> atmpObj;
        atmpObj.data = 5;
        cout << atmpObj.data << endl;
    }
};

int main(void)
{
    B<long> bl;
    bl.callBAF();

    B<int> bi;
    bi.callBAF();
}
```

如果是类模板，没有前向声明也不会报错。

### 让类型模板参数成为友元类

C++11 以后引入。

如果传递进来的模板参数是一个类类型，则这个类型可以称为当前类模板的友元类

```cxx
template <typename T>
class A2 {
    friend T;

private:
    int data;
};

class CF {
public:
    void callCFAF()
    {
        A2<CF> aObj1;  /*
						*	1. 实例化出来了 A2<CF>
						*	2. 让 CF 称为了 A2<CF> 的友元类
						*	3. 创建了一个对象
						*/
        aObj1.data = 12;
        cout << aObj1.data << endl;


		// A2<int> aObj2;
        // aObj2.data = 12;  // 编译错误
		/*
			因为CF类并不是 A2<int> 的友元类，自然不能在这里方位 aObj2 这个 A2<int> 类对象的 data 私有成员变量。
		*/
    }
};

int main(void)
{
    CF myCfObj;
    myCfObj.callCFAF();
}
```

如果传递给 类模板 A2 的类型模板参数不是一个类类型，那么代码行`friend T`就会被忽略

## 2 - 友元函数

### 让函数模板的某个实例成为友元函数

```cxx
template <typename U, typename V>
void func(U val1, V val2);

class Men {
private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }

	/* 这个 <int, int> 被称为 模板实参，当然，这里其实可以推断出来 */
    friend void func<int, int>(int, int);
    // friend void func<>(int, int); /* 但是尖括号不能省略 */
	/* 但是为了规范，还是建议加上 模板实参 */
};

template <typename U, typename V>
void func(U val1, V val2)
{
    Men mymen;
    mymen.funcmen();
    cout << "va1 = " << val1 << endl;
    cout << "va2 = " << val2 << endl;
}

int main(void)
{
    func(2, 3); // Men::funcmen 被调用了
}
```

### 友元模板

```cxx
/* Men 非模板版本 */
class Men {
private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }

    template <typename U, typename V>
    friend void func(U val1, V val2);
};

template <typename U, typename V>
void func(U val1, V val2)
{
    Men mymen;
    mymen.funcmen();
    cout << "va1 = " << val1 << endl;
    cout << "va2 = " << val2 << endl;
}

int main(void)
{
    func(2, 3); // Men::funcmen 被调用了
}
```

将 func 函数模板（泛化版本）声明为 Men 类模板的友元模板之后，
那么 func 函数模板的特化版本也会被看成是 Men 类模板的友元。

编译器会把 全特化的 func 函数模板 看待成一个实例化过的 函数模板。
所以在设置友元的时候，可能会出现一些问题。

而且偏特化可能还是有一些问题的，因为这相当于是重载了，
所以之前声明的友元可能并不会起作用。

### 在类模板中定义友元函数

这种友元函数是能够被调用的，而且也只有在代码中调用了函数的时候，编译器才会实例化出这个函数。
之所以这种定义友元函数，一般都是因为在该友元函数中会 用到这个类模板。
这种友元函数的调用与调用普通函数差不多，把他当做普通函数的调用即可。

```cxx
template <typename T>
class Men {
    /* 将 友元函数 定义在了类中，当做普通全局函数调用即可，这个甚至是忽视命名空间的 */
    friend void func2(Men<T>& tmpmen)
    {
        tmpmen.funcmen();
    }

private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }
};

int main(void)
{
    Men<int> aMenObj;
    func2(aMenObj);
}
```

因为 func2 在类模板 Men 中，所以调用 func2 时，
如果 func2 比较简单，那么编译器会把这个 func2 搞成 inline 的

下面是 C++ insight 给出的结果：

```cxx
template <typename T>
class Men {
public:
    friend inline void func2(Men<T>& tmpmen) { tmpmen.funcmen(); }

private:
    inline void funcmen() const
    {
        std::operator<<(
            std::cout,
            "Men::funcmen \350\242\253\350\260\203\347\224\250\344\272\206")
            .
            operator<<(std::endl);
    }
};
```
