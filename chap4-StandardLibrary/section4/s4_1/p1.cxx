#include <iostream>
#include <type_traits>

using namespace std;

template <int j>
using fuck = std::conditional<(j > 100), double,
    typename std::conditional<(j > 80 && j < 100), float,
        typename std::conditional<(j > 40 && j < 80), int,
            char>::type>::type>::type;

int main(void)
{
    // std::conditional<true, int, double>::type tmp1;
    // std::conditional<false, int, double>::type tmp2;

    // cout << "tmp1=" << typeid(tmp1).name() << endl; // int
    // cout << "tmp2=" << typeid(tmp2).name() << endl; // double

    fuck<50> tsVar = 100;
    cout << typeid(tsVar).name() << endl;
}