# chap4_4 - std::conditional

std::conditional 是 C++11 引入的类模板，表现编译期的分支逻辑

下面是 llvm - clang 的实现

```cxx
template <bool _Bp, class _If, class _Then>
struct conditional {
  using type = _If;
};

template <class _If, class _Then>
struct conditional<false, _If, _Then> {
  using type = _Then;
};
```

如果 `_Bp == true`，那么 type 这个类型别名代表的是 `_If`，否则代表的是`_Then`。
所以这个类模板看起来表达的是一种：if-then-else 的感觉。

能力：根据给进来的 bool 值不同，做到了让一个变量可能会有多重不同的类型的效果 ——
范例中变量可能是 int 类型，也可能是 double 类型

需求：定义一个名字叫做 tsVar 的变量，根据某个常量（取名为 j）值的不同，tsVar 变量的类型也不同。

1. 如果 j > 100，那么 tsVar 的类型是 double 类型
2. 如果 80 < j < 100，那么 tsVar 类型是 float 类型
3. 如果 40 < j < 80，那么 tsVar 类型是 int 类型
4. 如果 0 < j < 40，那么 tsVar 类型是 char 类型

编译器：6

```cxx
template <int j>
using fuck = std::conditional<(j > 100), double,
    typename std::conditional<(j > 80 && j < 100), float,
        typename std::conditional<(j > 40 && j < 80), int,
            char>::type>::type>::type;

int main(void)
{
    fuck<50> tsVar = 100;
    cout << typeid(tsVar).name() << endl;
}
```

总结：std::conditional 虽然使用不复杂，但是他通常适合实现 真或假 两种分支，
如果要支持多分支，可以看到，写出来的代码不美观。
