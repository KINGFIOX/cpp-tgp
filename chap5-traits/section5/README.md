# chap5_5 - 策略（policy）技术中的算法策略

求和就可以看做是一种算法，但是我们的 funcSum 实际上就把 求和写死了，这不好。
如果以后有：求最大值、最小值之类的，就不是很行

## 1 - 常规范例：普通策略类

策略类（普通的类）

```cxx
struct MinPolicy {
    template <typename sumT, typename T>
    static void algorithm(sumT& min, const T& value) /* 策略类的核心算法 */
    {
        min = min > value ? value : min;
    }
};
```

## 2 - 常规范例：策略类模板

策略类模板（类模板）

```cxx
template <typename sumT, typename T>
struct MinPolicy {
    static void algorithm(sumT& min, const T& value) /* 策略类的核心算法 */
    {
        min = min > value ? value : min;
    }
};
```

那么 funcSum 协程如下：

```cxx
template <typename T,
    typename U = SumFixedTraits<T>,
    template <class, class> class V = SumPolicy // 模板模板参数
    >
auto funcSum(const T* begin, const T* end)
{
    typename U::sumT result = U::initValue();

    for (;;) {
        V<typename U::sumT, T>::algorithm(result, *begin);
        // V::algorithm(result, *begin);
        // T 是 数组成员类型，U 是 固定萃取 类模板，
        // 从中可以提取出 结算的结果类型（U::sumT）以及结果的初值
        // V 是策略类模板，用于实现具体算法（求和，求最小值）
        if (begin == end) {
            break;
        }
        ++begin;
    }

    return result;
}
```

模板模板参数：

`template<class, class> (class/typename) V`，
这个 V 的类型是一个 类型，因此要带上`(class/typename)`；
然后这个模板的类型由模板的参数决定（就像函数类型由：函数返回值 + 函数参数决定）

## 3 - 萃取（trait）技术 与 策略（policy）技术的比较

两者之间的相同点、不同点：

1. 两种技术都想一个中间件一样，夹在不同的功能代码之间。让代码之间的调用更加灵活
2. 萃取技术：给进去一个类型，萃取出另外一个类型或者另外一个值（注重于 类型 或者 值）
3. 策略技术：给进去一个类型，萃取出一个不同的算法`V<typename U::sumT, T>::algorithm(result, *begin);`
   或者不同的功能实现（注重于动作或者行为）
   因此：在书写 策略类模板 的时候，通常都要包含成员函数 以及实现指定的行为

有时，萃取技术中也可能实现某些动作或者行为（比方说初始化值），
所以从这个角度来讲，萃取技术与策略技术又是区分不是那么明显。

萃取技术一般通过一个 类模板来实现。通常包含类模板的泛化版本 和 特化版本。
策略技术，通过普通的类也能实现，或者类模板都可以实现
