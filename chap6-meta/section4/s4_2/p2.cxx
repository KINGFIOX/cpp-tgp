#include <iostream>
#include <type_traits>
#include <utility>

#define Debug 1

/* ---------- ---------- 打印名字 ---------- ---------- */

#include <boost/type_index.hpp>

#define PrintVarType(var)                     \
    using boost::typeindex::type_id_with_cvr; \
    std::cout << type_id_with_cvr<decltype(var)>().pretty_name() << std::endl;

#define PrintType(T)                          \
    using boost::typeindex::type_id_with_cvr; \
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;

/* ---------- ---------- has mem func ---------- ---------- */

/* 泛化 */
template <typename T, typename U = std::void_t<>>
struct HasMemFunc : std::false_type { };

/* 特化 */
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc_MTL())>> : std::true_type { };

/* ---------- ---------- tuple ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam Types
 */
template <typename... Types>
class Tuple;

/**
 * @brief 特化版本 1
 *
 * @tparam First
 * @tparam Others
 */
template <typename First, typename... Others>
class Tuple<First, Others...> {
public:
    First first;
    Tuple<Others...> others; // 递归组合的方式，搞这个 tuple

public:
    /* 构造函数，支持创建 Tuple<int, float> myTuple(); 这种构造方式 */
    /* 给 < > 传了参数，但是 myTuple() 的 () 中间没有参数 */
    Tuple() { }

    /* 拷贝构造函数 */
    Tuple(const Tuple<First, Others...>& tmpObj)
        : first(tmpObj.first)
        , others(tmpObj.others)
    {
        std::cout << "Tuple 特化版本1 的拷贝构造函数执行了!" << std::endl;
        // 但是这会导致一个问题 tuple<double> = tuple<float> 会失败 ---> 拷贝构造函数模板
    }

    /* 允许 this 与 tmpObj 的类型不同 */
    template <typename C_First, typename... C_Others>
    Tuple(const Tuple<C_First, C_Others...>& tmpObj)
        : first(tmpObj.first)
        , others(tmpObj.others)
    {
        std::cout << "tuple 拷贝构造模板 执行了" << std::endl;
    }

    /* 万能引用，构造函数 */
    template <typename C_First, typename... C_Others,
        typename = std::enable_if_t<!HasMemFunc<C_First>::value>>
    Tuple(C_First&& parf, C_Others&&... paro)
        : first(std::forward<C_First>(parf))
        , others(std::forward<C_Others>(paro)...) // 注意这个 ... 在外面
    {
    }

public:
    void myfunc_MTL() { }
};

// tuple 有点像：上一节实现的 老式的 typelist

template <>
class Tuple<> {
public:
    Tuple()
    {
#if Debug
        m_sign = 0;
#endif
        std::cout << "Tuple 特化版本2 的构造函数执行了!" << std::endl;
    }
    Tuple(const Tuple<>&)
    {
#if Debug
        m_sign = 100;
#endif
        std::cout << "Tuple 特化版本2 的拷贝构造函数执行了!" << std::endl;
    }
#if Debug
    int m_sign;
#endif
};

int main(void)
{
    int i = 100;
    Tuple<float, int, std::string> t1(12.5f, i, std::string("fuck"));
    /* 发现有编译错误，因为调用到了万能引用了，解决方案：enable_if */

    Tuple<float, int, std::string> t2 = t1; //

    Tuple<double, int, std::string> t3 = t1; // tuple 拷贝构造模板 执行了
}