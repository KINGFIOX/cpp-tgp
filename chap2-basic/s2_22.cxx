#include <iostream>

using namespace std;

template <typename T = int, int arrsize = 20>
T g_myVar[arrsize];

int main(void)
{
    for (int i = 0; i < 15; i++) {
        g_myVar<int, 15>[i] = i;
    }

    for (int i = 0; i < 15; i++) {
        cout << g_myVar<int, 15>[i] << endl;
    }
    return 0;
}
