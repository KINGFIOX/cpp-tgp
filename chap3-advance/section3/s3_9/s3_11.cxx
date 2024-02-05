#include <iostream>
#include <string>
#include <type_traits>

using namespace std;

int main(void)
{
    cout << "string => float? " << std::is_convertible<string, float>::value << endl; // 0
    cout << "float => int? " << std::is_convertible<float, int>::value << endl; // 1
}