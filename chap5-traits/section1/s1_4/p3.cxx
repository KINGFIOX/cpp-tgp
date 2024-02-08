#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

int main(void)
{
    int a = 15;
    using boost::typeindex::type_id_with_cvr;
    typename std::add_lvalue_reference<decltype(a)>::type b = a;
    typename std::add_rvalue_reference<decltype(a)>::type c = 16;
    std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(b)>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(c)>().pretty_name() << std::endl;
}