#include <functional>
#if 1

#include <iostream>

using namespace std;

template <typename T>
class Men {
private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }

    // friend void func<int, int>(int, int);
    // friend void func<>(int, int);

    template <typename U, typename V>
    friend void func(U val1, V val2);
};

template <typename U, typename V>
void func(U val1, V val2)
{
    Men<int> mymen;
    mymen.funcmen();
}

// template <typename U>
// void func(int val1, U val2)
// {
//     Men<int> mymen;
//     mymen.funcmen();
// }

template <> /* 全特化版本 */
void func(int val1, double val2)
{
    cout << "---------- 全特化版本 ----------" << endl;
    Men<int> mymen;
    mymen.funcmen();
}

int main(void)
{
    func<float>(2, 3); // Men::funcmen 被调用了
    func(2, 2.3);
}

#else

#include <iostream>

using namespace std;

template <typename T>
class Men {
private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }

    // Declare func as a template friend function with two parameters.
    template <typename U, typename V>
    friend void func(U val1, V val2);
};

// Define func as a template function with three parameters.
template <typename T, typename U, typename V>
void func(U val1, V val2)
{
    Men<T> mymen;
    mymen.funcmen(); // Accesses private member of Men.
    cout << "va1 = " << val1 << endl;
    cout << "va2 = " << val2 << endl;
}

int main(void)
{
    func<float, int, int>(2, 3); // Now you need to specify all three template arguments.
}

#endif