#include <cstdio>
#include <iostream>

using namespace std;

template <typename... myCLSPList>
class myClassT : public myCLSPList... // 有一堆爹
{
public:
    myClassT()
        : myCLSPList()... /* 一堆爹用这种方式伺候 */
    {
        printf("myClassT::myClassT, this=%p\n", this);
    }
};

class PA1 {
public:
    PA1()
    {
        printf("PA1:PA1, this=%p\n", this);
    }

private:
    char m_s1[100];
};

class PA2 {
public:
    PA2()
    {
        printf("PA2:PA2, this=%p\n", this);
    }

private:
    char m_s2[200];
};

class PA3 {
public:
    PA3()
    {
        printf("PA3:PA3, this=%p\n", this);
    }

private:
    char m_s3[300];
};

int main(void)
{
    myClassT<PA1, PA2, PA3> Obj;
    cout << "sizeof(Obj) = " << sizeof(Obj) << endl;
    /*
            PA1:PA1, this=0x16fc36aa0 === myClassT （ this 指针调整 )
            PA2:PA2, this=0x16fc36b04
            PA3:PA3, this=0x16fc36bcc
            myClassT::myClassT, this=0x16fc36aa0
            sizeof(Obj) = 600
    */
}
