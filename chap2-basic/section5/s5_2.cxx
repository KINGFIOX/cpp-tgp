#include <iostream>

using namespace std;

class Men {
public:
    void eat()
    {
        cout << "男人喜欢吃面食" << endl;
    }
};

class Women {
public:
    void eat()
    {
        cout << "女人喜欢吃米饭" << endl;
    }
};

/**
 * @brief 函数模板
 *
 * @tparam T
 * @param obj
 */
template <typename T>
void eatTmp1(T& obj)
{
    obj.eat();
}

int main(void)
{
    Men objMen;
    Women objWomen;

    /* 这里自动推断了 */
    eatTmp1(objMen);
    eatTmp1(objWomen);
}