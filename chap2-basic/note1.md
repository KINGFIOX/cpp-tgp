# chap2 - 函数模板

## 1 - 基本范例

```cxx
template <typename T>
T Sub(T tv1, T tv2)
{
    return tv1 - tv2;
}
```

class 可以去到 typename，但是这里的 class 并没有 “类” 的意思

1. 模板的定义是以 template 关键字开头
2. 类型模板参数 T 前面用 typename 来修饰，所以，遇到 typename 就该知道其后面跟的是一个类型。
   typename 可以被 class 取代
3. 类型模板参数 T（代表的是一个类型）与 前面的修饰符 typname/class 都用`<>`括起来
4. T 这个名字可以换成任意其他标识符，对程序没有影响。用 T 只是一种编程习惯。

## 2 - 实例化

实例化：编译器用 具体的 “类型” 代替 “类型模板参数”（有人也叫代码生成器）

.o 文件在编译完成后就会产生

.o 文件的格式一般会被认为是一种 ELF 文件格式

可以看到有两个函数：

```sh
~/DOCs/cpp/cppTGP (main*) » objdump -d（显示汇编代码） -t（显示符号表） -C（去除名字修饰）  chap2-basic/s1_1.o
000000000000009c  w    F __TEXT,__text double Sub<double>(double, double)
000000000000007c  w    F __TEXT,__text int Sub<int>(int, int)
```

实例化之后的函数明分别叫做：`double Sub<double>(double, double)` 与 `int Sub<int>(int, int)`

通过函数模板实例化之后的函数明包含三个部分：

1. 模板名
2. 后面跟着一对`<>`
3. `<>`中间是一个具体类型

在编译阶段，编译器就会查看函数模板的 函数体 部分，来确定是否针对该类型 string 进行 Sub 函数模板的实例化。
所以：编译阶段，编译器需要能够找到函数模板的函数体部分。
所以：通常在写项目的时候，通常是将 函数头+函数体 写入 `.hxx` 文件中。

并且，我们这里确实是：`T Sub(T, T)`，我们需要：这两个类型是严格的相同的，
比方说`float subv2 = Sub(3.5, 4.0f);`这里，一个是 double，一个是 float，这就不行了。
我们这里其实是让编译器自动帮我们推断参数的类型，是拒绝一切形式的隐式转换！！！
如果不让编译器帮我们自动推断，那么确实是可以隐式转换的`float subv3 = Sub<double>(3, 4.0f);`。

## 3 - 模板参数的推断

比方说这段代码

```cxx
template <typename T, typename U, typename V>
V Add(T a, U b)
{
    return a + b;
}

int main(void)
{
    cout << Add(15, 17.8) << endl; // 错误，未找到的重载函数
}
```

为什么会有这种问题呢？因为编译器并没有办法推断出来 V 的类型，`15 + 17.8`，
这个会发生隐式转换，这个是一个运行时的行为（虽然我并不知道运行时行为的定义是什么）。
因此这个 V 是没法推断出来的

（有点明白这个运行时行为是什么意思了，我刚学完这个解释器模式）

既然是无法推断出来的，那么我们也可以指定出来嘛。
但还比较无奈的是：这个 V 是第三个模板参数，C++没有 “跳过前两个，指定第三个” 的语法规则`no Add<..., double>`。
为了让上面这个例子能够工作，可以：`Add<int, double, double>`。

那是否：把 v 放到前面，然后我们指定 V，就可以让编译器推断 T、U 了呢？

```cxx
template <typename V, typename T, typename U>
V Add(T a, U b)
{
    return a + b;
}

int main(void)
{
    cout << Add<double>(15, 17.8) << endl;  // 确实是可以的
}
```

当然：`Add<double, ..., int>`这种也是不允许的。

那 auto 关键字是否可以派上用场呢？

```cxx
template <typename T, typename U>
auto Add(T a, U b)
{
    return a + b;
}

int main(void)
{
    cout << Add<double>(15, 17.8) << endl;  // 这也确实是可以的
}
```

### 常规的参数推断

1. 通过`<>`可以只指定一部分函数模板的类型，另外一部分模板参数可以通过调用时给定的实参来推断

auto 代替函数模板返回类型（常见手法）

#### auto + decltype

decltype 可以与 auto 解释使用，来构成返回类型的后置语法。
这种后置语法其实也就是使用 auto 和 decltype 结合来完成返回值类型推导。

```cxx
template <typename T, typename U>
auto Add(T a, U b) -> decltype(a + b)
{
    return a + b;
}
```

（老师说：看他之前的教程）

#### 单走一个 auto

```cxx
template <typename T, typename U>
auto Add(T a, U b)  // 这里的 auto 就有 自动推导类型 的含义
{
    return a + b;
}
```

这里的`auto`其实并不是真正完成类型推导的，只是返回类型后置语法` -> decltype(a + b)`的组成部分

### 各种推断的比较以及空模板参数列表的推断

示例：

```cxx
template <typename T>
T mydouble(T tmpValue)
{
    return tmpValue * 2;
}
```

#### 编译器自动推断

给出实参

```cxx
int main(void)
{
	cout << mydouble(15) << endl;
}
```

#### 指定模板类型

```cxx
int main(void)
{
    cout << mydouble<int>(16.9) << endl;  // 输出结果是 32
}
```

指定类型 比 自动推断 优先级高

#### 指定空模板参数列表

```cxx
int main(void)
{
	cout << mydouble<>(16.9) << endl;  // 输出是 33.8
}
```

输出是 33.8，可见：编译器还是通过 “实参” 类型，自动推断出的模板参数。
此种场合下`<>`并没有什么用处，但是语法上运行。
当然这种情况下`<>`并没有太多实际的意义，那么什么时候`<>`有意义呢？

```cxx
template <typename T>
T mydouble(T tmpValue)
{
    cout << "template" << endl;
    return tmpValue * 2;
}

double mydouble(double tmpValue)
{
    cout << "double" << endl;
    return tmpValue * 2;
}

int main(void)
{
    cout << mydouble(16.9) << endl;  // double
	cout << mydouble<>(16.9) << endl;  // template
}
```

这种情况下：普通函数 与 函数模板并存，编译器会优先调用 普通函数。
这个时候`<>`就有作用了：告诉编译器，明明白白的调用函数模板

术语：模板函数 是通过 函数模板 生成的函数。（上面可能有笔误的地方）
