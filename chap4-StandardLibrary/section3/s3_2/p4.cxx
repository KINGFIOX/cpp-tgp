/**
 * @file p4.cxx
 * @author your name (you@domain.com)
 * @brief SFINAE 特性
 * @version 0.1
 * @date 2024-02-07
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

/* ----------  ----------  ---------- */

/* 泛化版本 */
template <typename, typename, typename = std::void_t<>> /* T, U, V 在实现体中用不到 */
// template <typename T, typename U, typename V = std::void_t<>>
struct IfCanAdd : std::false_type { };

/* 特化版本 */
template <typename T, typename U>
struct IfCanAdd<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : std::true_type { };

/* ----------  ----------  ---------- */

/* 泛化版本 */
template <typename T, typename U, bool ifcnado = IfCanAdd<T, U>::value>
struct VecAddResult {
    using type = decltype(std::declval<T>() + std::declval<U>());
};

/* 特化版本 */
template <typename T, typename U>
struct VecAddResult<T, U, false> { };

template <typename T, typename U>
using VecAddResult_t = typename VecAddResult<T, U>::type;

/* ----------  ----------  ---------- */

template <typename T, typename U>
std::vector<VecAddResult_t<T, U>> operator+(std::vector<T> const& vec1, std::vector<U> const& vec2)
{
    std::vector<VecAddResult_t<T, U>> tmpVec;

    return tmpVec;
}

struct elemC {
    elemC(int tmpValue) { }
    // elemC operator+(const elemC& tmppar);
};

int main(void)
{
    // auto m = int() + double();
    // cout << typeid(m).name() << endl;

    // std::vector<int> veca { 0, 10, 20 };
    // std::vector<double> vecb { 1.1, 2.2, 3.3 };
    // auto vecc = veca + vecb;

    // using boost::typeindex::type_id_with_cvr;
    // cout << type_id_with_cvr<decltype(vecc)>().pretty_name() << endl;
    // // std::__1::vector<double, std::__1::allocator<double>>

    vector<elemC> veca;
    vector<elemC> vecb;
    // auto vecc = veca + vecb; /* 错误： Invalid operands to binary expression ('vector<elemC>' and 'vector<elemC>') */
    auto vecc = veca + vecb; /* 错误： Invalid operands to binary expression ('vector<elemC>' and 'vector<elemC>') */
    using boost::typeindex::type_id_with_cvr;
    cout << type_id_with_cvr<decltype(vecc)>().pretty_name() << endl;
    // std::__1::vector<elemC, std::__1::allocator<elemC>>
}