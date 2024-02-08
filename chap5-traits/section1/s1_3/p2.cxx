#include <cstddef>
#include <cxxabi.h>
#include <iostream>
#include <list>
#include <vector>

/**
 * @brief 泛化版本不实现
 *
 * @tparam T
 */
template <typename T>
struct GetEleType {
    using type = typename T::value_type;
};

template <typename T>
struct GetEleType<std::vector<T>> {
    using type = T;
};

template <typename T>
struct GetEleType<std::list<T>> {
    using type = T;
};

/**
 * @brief 数组
 * （这个依然是：参数范围上的 偏特化）
 * @tparam T
 * @tparam Size 看起来这个特化版本多了一个 模板参数（但并非是 参数数量上的偏特化）
 */
template <typename T, std::size_t Size>
struct GetEleType<T[Size]> {
    using type = T;
    static const std::size_t size = Size;
};

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

/* ---------- ---------- print element type ---------- ---------- */

template <typename T>
void PrintEleType(const T& container)
{
    std::cout << typeid(GetEleType<T>::type).name() << std::endl;
}

int main(void)
{
    char fuck[] = "fuck";
    std::cout << typeid(GetEleType<decltype(fuck)>::type).name() << std::endl;
    std::cout << GetEleType<decltype(fuck)>::size << std::endl;
    std::vector<double> MyD;
    std::cout << typeid(decltype(MyD)::value_type).name() << std::endl; // d
}
