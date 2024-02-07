#include <iostream>

using namespace std;

template <class Tp>
struct rae {
    using type = Tp;
};

template <class Tp>
struct rae<Tp[]> {
    using type = typename rae<Tp>::type;
};

template <class Tp, size_t Np>
struct rae<Tp[Np]> {
    using type = typename rae<Tp>::type;
};

int main(void)
{
    double c[2][8][9]; // c 的类型是 double[2][8][9]

    cout << typeid(rae<decltype(c)>::type).name() << endl; // d
}