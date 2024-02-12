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

/* ---------- ---------- get ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam index
 */
template <int index>
class TupleGetIndex {
public:
    template <typename First, typename... Others>
    static auto myget(const Tuple<First, Others...>& tmpTuple)
    {
        return TupleGetIndex<index - 1>::myget(tmpTuple.others);
    }
};

template <>
class TupleGetIndex<0> {
public:
    template <typename First, typename... Others>
    static auto myget(const Tuple<First, Others...>& tmpTuple)
    {
        return tmpTuple.first;
    }
};

// 上面是函数式编程

template <int index, typename... Types>
auto TupleGet(const Tuple<Types...>& tmpTuple)
{
    return TupleGetIndex<index>::myget(tmpTuple);
}

/* ---------- ---------- make_tuple ---------- ---------- */

/**
 * @brief
 *
 * @tparam Types
 * @param Args
 * @return auto
 */
template <typename... Types>
auto MakeTuple(Types&&... Args)
{
    return Tuple<std::decay_t<Types>...>(std::forward<Types>(Args)...);
}

/* ---------- ---------- pop_front_type ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TL
 */
template <typename TL>
class pop_front_type;

template <typename FirstElem, typename... OtherElem>
class pop_front_type<Tuple<FirstElem, OtherElem...>> {
public:
    using type = Tuple<OtherElem...>;
    // 与 typelist 非常像
};

/* ---------- ---------- push_front ---------- ---------- */

#define rec 0

#if not rec

template <typename... ElemsTypes, typename NewType>
auto push_front(const Tuple<ElemsTypes...>& tmpTuple, const NewType& tmpvalue)
{
    // 跟 老式的 typelist 很像。others = tmpTuple，first = tmpvalue
    return Tuple<NewType, ElemsTypes...>(tmpvalue, tmpTuple);
}

#else

/**
 * @brief 向 空 tuple 插入元素 （base case）
 *
 * @tparam NewType
 * @param tmpTuple
 * @param tmpvalue
 * @return auto
 */
template <typename NewType>
auto push_front(const Tuple<>& tmpTuple, const NewType& tmpvalue)
{
    return Tuple<NewType>(tmpTuple);
}

template <typename FirstType, typename... OtherType, typename NewType>
auto push_front(const Tuple<FirstType, OtherType...>& tmpTuple, const NewType& tmpvalue)
{
    return Tuple<NewType, FirstType, OtherType...>(
        tmpvalue, push_front(tmpTuple.others, tmpTuple.first));
}

#endif

/* ---------- ---------- push_back ---------- ---------- */

/**
 * @brief 递归终止 base case
 *
 * @tparam NewType
 * @param tmpValue
 * @return auto
 */
template <typename NewType>
auto push_back(const Tuple<>&, const NewType& tmpValue)
{
    return Tuple<NewType>(tmpValue);
}

template <typename FirstType, typename... OtherType, typename NewType>
auto push_back(const Tuple<FirstType, OtherType...>& tmpTuple, const NewType tmpValue)
{
    return Tuple<FirstType, OtherType..., NewType>(tmpTuple.first, push_back(tmpTuple.others, tmpValue));
}

// 这个 push 也只有这个 递归的版本了，毕竟确实要往下翻的

/* ---------- ---------- pop_front ---------- ---------- */

template <typename... ElemsTypes>
auto pop_front(const Tuple<ElemsTypes...>& tmpTuple)
{
    return tmpTuple.others;
}

/* ---------- ---------- reverse ---------- ---------- */

#if 1

/**
 * @brief base case
 *
 * @return auto
 */
auto reverse(const Tuple<>&)
{
    return Tuple<>();
}

template <typename FirstType, typename... OtherType>
auto reverse(const Tuple<FirstType, OtherType...>& tmpTuple)
{
    return push_back(reverse(tmpTuple.others), tmpTuple.first);
    // 不过可以预见到：这个调用的时间复杂度是 O(n^2) 的，效率很低
    // 另一种解决方式：integer_sequence_reverse 类模板
}

#else

template <typename... Elems, std::size_t... Index>
auto LastReverse(const Tuple<Elems...>& tmpTuple, std::integer_sequence<std::size_t, Index...>)
{
    return MakeTuple(TupleGet<Index>(tmpTuple)...);
    // 但是实际上，这个实现成本还是挺高的，也是 O(n^2)
}

template <typename... Elems>
auto MidReverse(const Tuple<Elems...>& tmpTuple)
{
    return LastReverse(tmpTuple, IntegerSequence_Reverse_t<std::size_t, sizeof...(Elems)>());
}

#endif

/* ---------- ---------- 测试 ---------- ---------- */

#if 0

int main(void)
{
    int i = 100;
    Tuple<float, int, std::string> t1(12.5f, i, std::string("fuck"));
    /* 发现有编译错误，因为调用到了万能引用了，解决方案：enable_if */

    Tuple<float, int, std::string> t2 = t1; //

    Tuple<double, int, std::string> t3 = t1; // tuple 拷贝构造模板 执行了
}

#else

int main(void)
{
    int i = 100;
    // std::string fuck = "fuck";
    Tuple<float, int, std::string> t1(12.5f, i, 'c');
    char a = 'a';
    auto r2 = push_front(t1, a);
}

#endif
