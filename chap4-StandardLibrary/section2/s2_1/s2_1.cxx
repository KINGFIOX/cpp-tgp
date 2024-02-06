#include <iostream>
#include <string>
#include <type_traits>

using namespace std;

template <bool val>
struct BoolConstant {
    using type = BoolConstant; /* <==> using type = BoolConstant<val>; 也行 */
    static constexpr bool value = val; /* 标准库源码中也通常会有这一行 */
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

template <typename T, bool val>
struct AClass {
    AClass()
    {
        cout << "AClass:AClass() 执行了!" << endl;
        // if constexpr /* ✅ */ (val) {
        // if (val) {
        //     //
        //     T tmpa = 15;
        //     /* 错误：No viable conversion from 'int' to */
        // } else {
        //     //
        //     T tmpa = "abc";
        //     /* 错误：Cannot initialize a variable of type 'int' with an lvalue of type 'const char[4]' */
        // }

        ACLassEX(BoolConstant<val>());
    }

    void ACLassEX(TrueType abc)
    {
        T tmpa = 15;
    }

    void ACLassEX(FalseType abc)
    {
        T tmpa = "abc";
    }
};

int main(void)
{
    AClass<int, true> a;
    AClass<string, false> b;
}
