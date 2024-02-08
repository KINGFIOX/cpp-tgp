#include <boost/type_index.hpp>
#include <iostream>

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

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

int main(void)
{
    RemoveConst_t<const int> nca = 15;
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<decltype(nca)>().pretty_name() << std::endl;
}