#include <iostream>

using namespace std;

template <typename T>
class A {
public:
    template <typename U>
    friend class B;

private:
    int data;
};

template <typename U>
class B {
public:
    void callBAF()
    {
        A<int> atmpObj;
        atmpObj.data = 5;
        cout << atmpObj.data << endl;
    }
};

int main(void)
{
    B<long> bl;
    bl.callBAF();

    B<int> bi;
    bi.callBAF();
}
