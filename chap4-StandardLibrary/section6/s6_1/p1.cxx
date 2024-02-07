#include <iostream>
#include <type_traits>

using namespace std;

int main(void)
{
    float a[12]; // a 的类型 float [12]
    int b[5][8]; // b 的类型 int [5][8]
    double c[2][8][9]; // c 的类型是 double[2][8][9]

    cout << typeid(decltype(c)).name() << endl; // A2_A8_A9_d
    cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << endl; // d
}