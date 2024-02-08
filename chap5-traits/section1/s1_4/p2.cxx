#include <boost/type_index.hpp>
#include <iostream>

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

int main(void)
{
    using boost::typeindex::type_id_with_cvr;
    int a1 = 12;
    int& a2 = a1;
    int&& a3 = 12;
    std::cout << type_id_with_cvr<decltype(a3)>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<RemoveReference_t<decltype(a3)>>().pretty_name() << std::endl;
}