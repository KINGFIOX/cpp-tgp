#include <iostream>

using namespace std;

class Human {
public:
    virtual void eat()
    {
        cout << "人类以吃米饭和面食为主!" << endl;
    }
    virtual ~Human() { }
};

class Men : public Human {
public:
    virtual void eat()
    {
        cout << "男人喜欢吃面食" << endl;
    }
};

class Women : public Human {
public:
    virtual void eat()
    {
        cout << "女人喜欢吃米饭" << endl;
    }
};

int main(void)
{
    Men objMen;
    Women objWomen;

    /* 父类引用子类对象 */
    Human& yinBase1 = objMen;
    Human& yinBase2 = objWomen;

    yinBase1.eat();
    yinBase2.eat();
}