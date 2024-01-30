#include <iostream>

using namespace std;

/**
 * @brief 类模板定义
 *
 * @tparam T
 */
template <typename T>
class MyVector {
public:
    typedef T* myIterator;

public:
    MyVector(T v);
    // MyVector<int>& operator=(const MyVector&); // 在类定义中，可以被省略
    MyVector& operator=(const MyVector&); // 赋值运算符重载

public:
    void myfunc()
    {
        cout << "myfunc()被调用" << endl;
    }

public:
    myIterator mybegin();
    myIterator myend();
};

/**
 * @brief Construct a new My Vector< T>:: My Vector object 类外实现构造函数
 *
 * @tparam T
 */
template <typename T>
MyVector<T>::MyVector(T v)
{
}

int main(void)
{
    // MyVector<int> tempvec; // T 被替换成了 int
    MyVector tmpvec2(12);
    // tempvec.myfunc();
}