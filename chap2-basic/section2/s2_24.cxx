#include <iostream>

using namespace std;

template <typename T>
class D {
public:
    template <typename W>
    static W m_tpi; // 静态成员变量模板声明
};

template <typename T>
template <typename W>
W D<T>::m_tpi = { 5 };

int main(void)
{
    cout << D<float>::m_tpi<int> << endl; // 5
    D<float>::m_tpi<int> = 150;
    cout << D<float>::m_tpi<int> << endl; // 150
    cout << D<int>::m_tpi<int> << endl; // 5
    cout << D<int>::m_tpi<float> << endl; // 5
}
