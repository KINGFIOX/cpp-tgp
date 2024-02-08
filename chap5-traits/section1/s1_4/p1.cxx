#include <iostream>
#include <type_traits>

template <class T1, class T2>
void print_is_same()
{
    std::cout << typeid(T1).name() << std::endl;
    std::cout << typeid(T2).name() << std::endl;
    std::cout << "T1 == T2: " << std::is_same<T1, T2>() << std::endl; // () 产生了临时对象，并转换为 int
    /* 类型转换运算符 constexpr opeator value_type() const noexcept {}; */
    // std::cout << "T1 == T2" << std::is_same<T1, T2>::value << std::endl; 这两个写法一样的
}

int main(void)
{
    std::remove_reference<int>::type a;
    std::remove_reference<int&>::type b;
    std::remove_reference<int&&>::type c;

    print_is_same<decltype(a), decltype(b)>();
    print_is_same<decltype(a), decltype(c)>();
}