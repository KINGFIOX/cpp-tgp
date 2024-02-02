#include <ios>
#include <iostream>

using namespace std;

template <typename T>
struct shape {
    /**
     * @brief 在类模板中定义友元函数，相当于是定义全局函数
     *
     */
    friend bool operator==(const shape<T>& obj1, const shape<T>& obj2)
    {
        const T& objtmp1 = static_cast<const T&>(obj1);
        const T& objtmp2 = static_cast<const T&>(obj2);

        if (objtmp1 < objtmp2 == false && objtmp2 < objtmp1 == false) {
            return true;
        } else {
            return false;
        }
    }
};

/**
 * @brief 正方形
 *
 */
struct square : public shape<square> {
    int sidelen;
};

/**
 * @brief 类外重载运算符
 *
 */
bool operator<(square const& obj1, square const& obj2)
{
    if (obj1.sidelen < obj2.sidelen) {
        return true;
    } else {
        return false;
    }
}

int main(void)
{
    square sq1;
    square sq2;

    sq1.sidelen = 15;
    sq2.sidelen = 15;

    cout << (sq1 == sq2) << endl;
}