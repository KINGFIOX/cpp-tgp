#include <iostream>

using namespace std;

template <typename T>
struct B {
    const static T value = { 160 };
    // constexpr static T value = { 160 }; 也是可以的
};

template <typename T>
int g_myVar = B<T>::value;

int main(void)
{
    cout << g_myVar<int> << endl; // 160
    g_myVar<int> = 152;
    cout << g_myVar<int> << endl; // 152
    cout << B<int>::value << endl; // 160
    return 0;
}
