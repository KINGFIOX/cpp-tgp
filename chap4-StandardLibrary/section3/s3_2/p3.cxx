#include <boost/type_index.hpp>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

template <typename T, typename U>
struct VecAddResult {
    // using type = decltype(T() + U()); /* 把结果类型的推导，交给了编译器 */
    using type = decltype(std::declval<T>() + std::declval<U>()); /* 把结果类型的推导，交给了编译器 */
};

template <typename T, typename U>
using VecAddResult_t = typename VecAddResult<T, U>::type;

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