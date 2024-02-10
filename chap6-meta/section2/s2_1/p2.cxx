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

/* ---------- ---------- 计算阶乘 ---------- ---------- */

template <int n_v>
struct Factorial {
    enum {
        value = n_v * Factorial<n_v - 1>::value
    };
};

/**
 * @brief 递归 base case
 *
 * @tparam
 */
template <>
struct Factorial<1> {
    enum {
        value = 1
    };
};

/* ---------- ---------- 模板元编程 ---------- ---------- */

template <typename T>
struct AddPoint {
    using type = T*;
};

/* ---------- ---------- 点积 ---------- ---------- */

/**
 * @brief 计算 点积
 *
 * @tparam T
 * @tparam U
 * @param arr1
 * @param arr2
 * @return auto
 */
template <typename T, int U>
inline auto DotProduct(T* arr1, T* arr2)
{
    T dpResult = T {}; // 零初始化
    for (int i = 0; i < U; ++i) {
        dpResult += arr1[i] * arr2[i];
    }
    return dpResult;
}

// 加了 inline 以后，就没有调用了，是直接被替换了

int main(void)
{
    typename AddPoint<const char>::type s = "fuck you";
    std::cout << typeName<decltype(s)>() << std::endl;
}