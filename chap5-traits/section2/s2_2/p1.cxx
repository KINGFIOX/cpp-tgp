#include <boost/type_index.hpp>
#include <iostream>
#include <type_traits>

using namespace std;

int main(void)
{
    cout << std::is_void<int>::value << endl; // 0
    cout << std::is_void<void>::value << endl; // 1
}