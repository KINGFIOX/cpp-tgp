#include <cstddef>
#include <iostream>

using namespace std;

template <typename T, typename U, auto arrsize = 8>
struct TC {
    T m_arr[arrsize]; /* 因为 arrsize 是：编译期间就确定的 */
    void functest();
};

// /* 错误： arrsize: Cannot add a default template argument to the definition of a member of a class template */
// template <typename T, typename U, size_t arrsize = 8>
// void TC<T, U, arrsize>::functest()
// {
// }

template <typename T, typename U, auto arrsize>
void TC<T, U, arrsize>::functest()
{
    cout << "functest 泛化版本" << endl;
}

int main(void)
{
    TC<double, double> mytc1;
    for (size_t i = 0; i < 8; ++i) {
        mytc1.m_arr[i] = static_cast<double>(i);
    }
    cout << mytc1.m_arr[7] << endl; // 7

    TC<double, double, 18> mytc2;
    mytc2.m_arr[10] = 16.8 / 0.0; // inf
    cout << mytc2.m_arr[10] << endl;
}