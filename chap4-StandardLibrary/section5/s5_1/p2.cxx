#include <functional>
#include <iostream>
#include <utility>

using namespace std;

/* ---------- ---------- ---------- */

/* 可调用对象处理器 */
template <typename T, typename... Args>
class CFObjHandler {
public:
    virtual T invoke(Args... args) const { } /* 虚函数，后续要创建子类 */
};

template <typename U, typename T, typename... Args>
class CFObjHandlerChild : public CFObjHandler<T, Args...> {
public:
    /* 这里只是右值引用，并不是万能引用（那左值引用行不行？） */
    CFObjHandlerChild(U&& tmpFuncObj)
        // CFObjHandlerChild(U& tmpFuncObj)
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
    CallFuncObj(U&& acObj) /* 万能引用，但是要有一个约束：有成员函数 operator() */
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

void callObjFunc2(CallFuncObj<void(int)> cObj)
{
    cObj(120); /* 在 CallFuncObj 中增加成员函数 */
}

void myfunc(int tmpValue)
{
    cout << "myfunc 执行了: tmpValue = " << tmpValue << endl;
}

int main(void)
{
    callObjFunc2(myfunc);
    callObjFunc2([](int tmpValue) -> void {
        cout << "lambda 表达式 执行了: tmpValue = " << tmpValue << endl;
    });

    CallFuncObj<void(int)> f1 = myfunc;
    f1(1200); // myfunc 执行了: tmpValue = 1200
    return 0;
}