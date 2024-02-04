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

// template <typename T>
// void func(T&& tmprv)
// {
//     cout << tmprv << endl;
//     return;
// }

template <typename T>
void func(const T&& tmprv)
/**
 * 1. 有 const 修饰，就不再是万能引用了（万能引用的资格被剥夺）
 * 2. 又因为有 &&，因此这不过是一个右值引用
 */
{
}

int main(void)
{
    func(100);
    int i = 100;
    // func(i); /* no matching function for call to 'func' */
}