/**
 * @file p3.cxx
 * @author your name (you@domain.com)
 * @brief 混合元编程，范例
 * @version 0.1
 * @date 2024-02-10
 *
 * @copyright Copyright (c) 2024
 *
 */

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

/* ---------- ---------- 点积 ---------- ---------- */

template <typename T, int U>
struct DotProduct {
    static T result(const T* a, const T* b)
    {
        // 元素个数减少，并且向后跳一步
        return (*a) * (*b) + DotProduct<T, U - 1>::result(a + 1, b + 1);
    }
};

template <typename T>
struct DotProduct<T, 0> {
    static T result(const T* a, const T* b)
    {
        return T {};
    }
};

int main(void)
{
    int a[] = { 1, 2, 3 };
    int b[] = { 3, 4, 5 };
    int result = DotProduct<int, 3>::result(a, b);

    std::cout << result << std::endl;

    return 0;
}