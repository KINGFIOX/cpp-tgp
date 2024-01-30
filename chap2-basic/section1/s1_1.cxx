#include <cstdio>
#include <iostream>

using namespace std;

// int Sub(int tv1, int tv2)
// {
//     return tv1 - tv2;
// }

template <typename T>
T Sub(T tv1, T tv2)
{
    return tv1 - tv2;
}

/* ----------  ---------- */

// template <typename T, typename U, typename V>
// V Add(T a, U b)
// {
//     return a + b;
// }

/* ----------  ---------- */

// template <typename V, typename T, typename U>
// V Add(T a, U b)
// {
//     return a + b;
// }

// template <typename T, typename U>
// auto Add(T a, U b)
// {
//     return a + b;
// }

template <typename T, typename U>
auto Add(T a, U b) -> decltype(a + b)
{
    return a + b;
}

/* ----------  ---------- */

template <typename T>
T mydouble(T tmpValue)
{
    cout << "template" << endl;
    return tmpValue * 2;
}

double mydouble(double tmpValue)
{
    cout << "double" << endl;
    return tmpValue * 2;
}

int main(void)
{
    int subv1 = Sub(3, 5);
    printf("subv1 = %d\n", subv1);

    float subv2 = Sub(3.5f, 4.0f);
    printf("subv2 = %f\n", subv2);

    // float subv3 = Sub<double>(3, 4.0f);

    cout << Add<double>(15, 17.8) << endl;

    cout << mydouble(15) << endl;

    cout << mydouble<int>(16.9) << endl;

    cout << mydouble<>(16.9) << endl;
    cout << mydouble(16.9) << endl;

    return 0;
}