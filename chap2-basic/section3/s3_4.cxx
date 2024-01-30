#include <iostream>

using namespace std;

template <typename T>
class Men {
    /* 将 友元函数 定义在了类中，当做普通全局函数调用即可，这个甚至是忽视命名空间的 */
    friend void func2(Men<T>& tmpmen)
    {
        tmpmen.funcmen();
    }

private:
    void funcmen() const
    {
        cout << "Men::funcmen 被调用了" << endl;
    }
};

int main(void)
{
    Men<int> aMenObj;
    func2(aMenObj);
}