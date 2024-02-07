# chap4_5 - std::functional

## 1 - 讲述标准库中的 std::function

是可调用对象包装器，是 C++11 新标准引入的类模板。

可调用对象：函数名、函数指针、具有 operator()成员函数的类对象、lambda 表达式

std::function 的强大能力：接受各种可调用对象，并进行实际的调用

## 2 - 实现类似 std::function 的功能

是怎么做：可接受任何可调用对象。

```cxx
/* 可调用对象处理器 */
template <typename T, typename... Args>
class CFObjHandler {
public:
    virtual T invoke(Args... args) const { } /* 虚函数，后续要创建子类 */
};

template <typename U, typename T, typename... Args>
class CFObjHandlerChild : public CFObjHandler<T, Args...> {
public:
    /* TODO 这里只是右值引用，并不是万能引用（那左值引用行不行？） */
    CFObjHandlerChild(U&& tmpFuncObj)
        : functor(std::forward<U>(tmpFuncObj))
    {
    }

    T invoke(Args... args) const override
    {
        return functor(std::forward<Args>(args)...);
    }

private:
    U functor; // U 是一个可调用对象类型，functor 用于保存可调用对象
};

/* ---------- ---------- ---------- */

/* 泛化版本，仅声明，因为用不到 */
template <typename T>
class CallFuncObj;

/* 特化版本 */
template <typename T, typename... Args>
/* T(Args...) ：返回 T ，参数 Args... */
class CallFuncObj<T(Args...)> {
public:
    /* 为了能够接受参数：构造函数 */
    template <typename U>
    CallFuncObj(U&& acObj)
    {
        /* 构造函数，怎么存起来这个 acObj */
        handler = new CFObjHandlerChild<U, T, Args...>(std::forward<U>(acObj));
    }

    ~CallFuncObj()
    {
        delete handler;
    }

    T operator()(Args... args) const
    {
        /* 完美转发，保持 原始参数的 左值右值性 */
        return handler->invoke(std::forward<Args>(args)...);
    }

private:
    CFObjHandler<T, Args...>* handler;
};
```

所以说上面 CFObjHandlerChild，为什么一定得是：右值引用？

首先是引用，这是毋庸置疑的，避免对象的拷贝。
那左值引用行不行呢？

经过尝试：如果用左值引用，lambda 表达式就会错误（其他有名函数就没啥问题）

```cxx
class CFObjHandlerChild {
    CFObjHandlerChild(U& tmpFuncObj) : functor(std::forward<U>(tmpFuncObj)) { }
};

class CallFuncObj{
public:
    CallFuncObj(U&& acObj) /* 万能引用，但是要有一个约束：有成员函数 operator() */
    {
        /* 构造函数，怎么存起来这个 acObj */
        handler = new CFObjHandlerChild<U, T, Args...>(std::forward<U>(acObj));
    }
};

error: p2.cxx:51:23: error: no matching constructor for initialization of 'CFObjHandlerChild<(lambda at p2.cxx:82:18), void, int>'
   51 |         handler = new CFObjHandlerChild<U, T, Args...>(std::forward<U>(acObj));
      |                       ^                                ~~~~~~~~~~~~~~~~~~~~~~
```

如果传进来是 临时的 lambda 表达式，临时的 lambda 表达式是右值。
那么也就是`handler = new CFObjHandlerChild<M&&, T, Args...>(std::forward<M&&>(lambda));`。
然后到`CFObjHandlerChild`，他的构造函数是`CFObjHandlerChild(M&& & tmpFuncObj)`，
经过引用折叠，其构造函数就是`CFObjHandlerChild(M& tmpFuncObj)`。
这就导致了那个错误：没有匹配的构造函数。

但是`&& && ---> &&`是个右值，`& && ---> &`是个左值引用
