#include <iostream>

using namespace std;

template <typename T, typename U = T*>
struct TC {
    /* ... */
};

int main(void)
{
    TC<double> mytc5; /* <double, double*> */
    TC<double, int> mytc6; /* <double, int> */
}