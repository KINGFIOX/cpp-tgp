#include <iostream>

using namespace std;

/**
 * @brief Base 是类模板
 *
 * @tparam T
 */
template <typename T>
class Base {
public:
    void asDerived()
    {
        /**
         * @brief 把自己转换为 派生类对象，
         * 		在单继承下，父类对象与派生类对象首地址对齐的
         *
         */
        T& derived = static_cast<T&>(*this);
        derived.func(); /* 调用派生类对象的成员函数 */
    }

private:
    /**
     * @brief 解决笔误的问题：私有化构造函数、派生类作为友元。这就是为了防止写错
     *
     */
    Base() { }
    friend T;
};

/**
 * @brief 普通的类
 *
 */
class Derived1 : public Base<Derived1> {
public:
    void func()
    {
        cout << "Derived::func() 执行了" << endl;
    }
};

/**
 * @brief 类模板
 *
 * @tparam T
 */
template <typename T>
class Derived2 : public Base<Derived2<T>> {
};

/**
 * @brief 假设这里的 Base<Derived3> 不小心写成了 Base<Derived1>
 *			 如何防止这种笔误的错误发生？
 */
class Derived3 : public Base<Derived1> {
public:
};

int main(void)
{
    Derived1 myd;
    myd.asDerived(); // Derived::func() 执行了

    /* 错误： Default constructor of 'Derived3' is implicitly deleted
                because base class 'Base<Derived1>' has an inaccessible default constructor */
    Derived3 myd3;
}
