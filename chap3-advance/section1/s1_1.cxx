#include <cstdio>
#include <iostream>

using namespace std;

// template <typename T> // T = int； abc = const int &
// void func(const T& abc)
// /* 注意：T的类型，不仅取决于：实参的类型，还取决于 const T& */
// {
// }

// void func(int&& tmprv) /* 参数tmprv是一个右值引用 */
// {
//     printf("%d", tmprv);
// }

template <typename T>
void func(T&& tmprv)
{
    cout << tmprv << endl;
    return;
}

int main(void)
{
    func(10);

    int&& rv = 1000;
    printf("%p\n", &rv); // 0x16d8aecbc
    rv = 10;
    printf("%p\n", &rv); // 0x16d8aecbc

    func(100);
    int i = 100;
    func(i);
}