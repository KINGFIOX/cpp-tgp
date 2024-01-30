#include <iostream>

using namespace std;

// template <typename T, int value>
template <typename, int> /* 这种语法是允许的，具体如何使用，后面体会 */
// template <class T, typename int value> /* expected a qulified name after 'typename' */
auto Add()
{
    return 100;
}

int main(void)
{
    cout << Add<int, 10>() << endl;
}