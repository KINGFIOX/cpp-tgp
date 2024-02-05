#include <iostream>
#include <type_traits>

using namespace std;

template <typename T>
struct MEB {
    using type = T;
};

int main(void)
{
    MEB<int>::type abc = 15; // MEB<int>::type 就代表了 int

    std::enable_if<(3 > 2)>::type* pointer1 = nullptr; // 没有第二个模板参数，那么默认是void

    std::enable_if<(3 < 2)>::type* pointer2 = nullptr;
}