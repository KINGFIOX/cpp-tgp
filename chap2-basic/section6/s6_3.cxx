#include <iostream>

using namespace std;

template <typename T>
class Human {
public:
    T& toChild()
    {
        return static_cast<T&>(*this);
    }

    void parentEat()
    {
        toChild().eat();
    }

private:
    Human() { }
    friend T;
};

class Men : public Human<Men> {
public:
    void eat()
    {
        cout << "男人喜欢吃面食" << endl;
    }
};

class Women : public Human<Women> {
public:
    void eat()
    {
        cout << "女人喜欢吃米饭" << endl;
    }
};

template <typename T>
void myHumanFuncTest(Human<T>& tmpobj)
{
    tmpobj.parentEat();
}

int main(void)
{
    Men menObj;
    Women womenObj;

    menObj.parentEat(); // 男人喜欢吃面食
    womenObj.parentEat(); // 女人喜欢吃米饭

    cout << "----------  ----------" << endl;

    myHumanFuncTest(menObj); // 男人喜欢吃面食
    myHumanFuncTest(womenObj); // 女人喜欢吃米饭
}