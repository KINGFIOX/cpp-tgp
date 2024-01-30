#include <iostream>

using namespace std;

template <typename T>
class A2 {
    friend T;

private:
    int data;
};

class CF {
public:
    void callCFAF()
    {
        A2<CF> aObj1;
        aObj1.data = 12;
        cout << aObj1.data << endl;

        // A2<int> aObj2;
        // aObj2.data = 12;  // 编译错误
    }
};

int main(void)
{
    CF myCfObj;
    myCfObj.callCFAF();
}