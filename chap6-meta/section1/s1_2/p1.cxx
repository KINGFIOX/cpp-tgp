#include <cxxabi.h>
#include <iostream>

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

/* ---------- ---------- 打印名字 ---------- ---------- */

template <typename T>
struct AddPoint {
    using type = T*;
};

int main(void)
{
    typename AddPoint<const char>::type s = "fuck you";
    std::cout << typeName<decltype(s)>() << std::endl;
}