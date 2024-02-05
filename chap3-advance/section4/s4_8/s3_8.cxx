#include <iostream>

using namespace std;

template <typename T>
typename T::size_type myDouble(const T& t)
{
    return t[0] * 2;
}

// int myDouble(int i)
// {
//     return i * 2;
// }

int main(void)
{
    myDouble(15);
}