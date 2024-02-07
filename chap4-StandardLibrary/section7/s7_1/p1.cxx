#include <cxxabi.h>
#include <iostream>
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

/* ---------- ---------- 打印名字 ---------- ---------- */

using namespace std;

int main(void)
{
    std::make_index_sequence<4> tmpobj2;
    cout << typeid(tmpobj2).name() << endl;
    cout << typeName<decltype(tmpobj2)>() << endl;
}