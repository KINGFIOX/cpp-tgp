#include <boost/type_index.hpp>
#include <cstdio>
#include <cxxabi.h>
#include <iostream>
#include <type_traits>
#include <utility>

/* ---------- ---------- 打印名字 ---------- ---------- */

std::string demangle(const char* mangledName)
{
    int status = 0;
    std::unique_ptr<char, void (*)(void*)> result(
        abi::__cxa_demangle(mangledName, nullptr, nullptr, &status),
        std::free);
    return (status == 0) ? result.get() : mangledName;
}

template <typename T>
std::string typeName()
{
    return demangle(typeid(T).name());
}

/* ---------- ---------- 测试代码 ---------- ---------- */

void mf(int&)
{
    std::cout << "call int&" << std::endl;
}

void mf(int&&)
{
    std::cout << "call int&&" << std::endl;
}

template <typename T>
void myfunc(T&& tmprv)
{
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(tmprv)>().pretty_name() << std::endl;
}

int main(void)
{
    // // int i = 10;
    // // int&& j = i;  // 右值引用 只能绑定 右值
    // int i = 10;
    // int&& j = std::forward<int>(i);
    // j = 50;
    // std::cout << i << std::endl; // 50

    // mf(std::forward<int>(i));

    int b = 10;
    int&& a = 10;
    // // int&& b = a; /* 错误： Rvalue reference to type 'int' cannot bind to lvalue of type 'int' */

    mf(a); // call int&
    mf(std::forward<int>(a)); // call int&&
    mf(std::forward<int&&>(a)); // call int&&
    mf(std::forward<int>(b)); // call int&&
}
