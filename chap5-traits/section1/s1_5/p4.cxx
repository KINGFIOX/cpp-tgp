#include <boost/type_index.hpp>
#include <iostream>

template <typename T>
void func(T tmpRv)
{
    std::cout << "---------- begin ----------" << std::endl;
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(tmpRv)>().pretty_name() << std::endl;
    std::cout << "---------- end ----------" << std::endl;
}

using boost::typeindex::type_id_with_cvr;

/* ---------- 删除 引用 ---------- */

template <typename T>
struct RemoveReference {
    using type = T;
};

template <typename T>
struct RemoveReference<T&> {
    using type = T;
};

template <typename T>
struct RemoveReference<T&&> {
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

/* ---------- 删除 const ---------- */

/**
 * @brief 泛化版本
 *
 */
template <typename T>
struct RemoveConst {
    using type = T;
};

/**
 * @brief 特化版本
 *
 */
template <typename T>
struct RemoveConst<const T> {
    using type = T;
};

/* ---------- remove CR ---------- */

/**
 * @brief 把 const 和 引用 去掉
 *
 * @tparam T
 */
template <typename T>
struct RemoveCR : RemoveConst<typename RemoveReference<T>::type> { };
// 1. RemoveReference<T>::type 去掉 引用 的类型
// 2. RemoveConst<typename RemoveReference<T>::type> 中有成员 type
// 3. RemoveCR 继承 RemoveConst<typename RemoveReference<T>::type>，相当于是继承了成员 type
// 个人想法：也可以 using type = RemoveConst<typename RemoveReference<T>::type>::type 实现

template <typename T>
using RemoveCR_t = typename RemoveCR<T>::type;

template <typename T>
struct RMCR {
    using type = RemoveConst<typename RemoveReference<T>::type>::type;
};

template <typename T>
using RMCR_t = typename RMCR<T>::type;

/* ---------- decay ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam T
 */
template <typename T>
struct Decay : RMCR<T> { };

/**
 * @brief 针对 (有边界)数组 的特化版本，这不继承 RMCR
 *
 * @tparam T
 * @tparam Size
 */
template <typename T, std::size_t Size>
struct Decay<T[Size]> {
    using type = T*;
};

/**
 * @brief 针对 (无边界)数组 的特化版本，这不继承 RMCR
 *
 * @tparam T
 */
template <typename T>
struct Decay<T[]> {
    using type = T*;
};

template <typename T, typename... Args>
struct Decay<T(Args...)> {
    using type = T (*)(Args...);
};

template <typename T>
using Decay_t = Decay<T>::type;

/* ----------  ---------- */

void testFunc2()
{
    std::cout << "testFunc2 执行了" << std::endl;
}

void f2()
{
    std::cout << "f2 执行了" << std::endl;
}

int main(void)
{
    /* 1. 去掉修饰符 */
    std::decay<const int&>::type nb1 = 26; /* nb1 类型为：int */
    std::cout << type_id_with_cvr<decltype(nb1)>().pretty_name() << std::endl;

    // /* 2. 数组 */
    // std::decay<int[10]>::type nb2; /* nb2 类型为：int* */
    // std::cout << type_id_with_cvr<decltype(nb2)>().pretty_name() << std::endl;

    /* 2. 数组 */
    Decay<int[10]>::type nb2; /* nb2 类型为：int* */
    std::cout << type_id_with_cvr<decltype(nb2)>().pretty_name() << std::endl;

    /* 测试 */
    RMCR<const int&>::type nb3 = 26;
    std::cout << type_id_with_cvr<decltype(nb3)>().pretty_name() << std::endl;

    Decay<decltype(testFunc2)>::type f2;
    std::cout << type_id_with_cvr<decltype(f2)>().pretty_name() << std::endl; // void(*)()
    // f2(); // 错误：execv(./chap5-traits/section1/s1_5/build/p4 ) failed(-1)
    f2 = testFunc2;
    f2();
}