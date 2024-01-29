#include <iostream>

using namespace std;

// template <typename T = int, typename U> /* 错误：U: Template parameter missing a default argument */
// struct TC {
//     TC()
//     {
//         cout << "TC 泛化版本的构造函数" << endl;
//     }

//     void functest1()
//     {
//         cout << "functest1 泛化版本" << endl;
//     }
// };

template <typename T = int, typename U = float>
struct TC {
    TC()
    {
        cout << "TC 泛化版本的构造函数" << endl;
    }

    void functest1()
    {
        cout << "functest1 泛化版本" << endl;
    }
};

int main(void)
{
    TC<> mytc5; /* <default, default> */
    TC<double> mytc6; /* <double, default> */
    TC<double, double> mytc7; /* <double, double> */
    return 0;
}

// /**
//  * @brief 偏特化版本（不能有缺省值！）
//  *
//  * @tparam T
//  */
// template <typename T = char> /* 错误： Default template argument in a class template partial specialization */
// struct TC<T, int> {
// };