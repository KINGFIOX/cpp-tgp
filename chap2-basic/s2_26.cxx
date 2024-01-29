#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <
    typename T,
    // typename Container = vector
    // template <class> class Container = vector // 这就是一个模板模板参数，写法比较固定，这里的名字叫Container，叫U也可以
    // template <typename W> class Container = vector // 这就是一个模板模板参数，写法比较固定，这里的名字叫Container，叫U也可以
    template <typename W, typename = std::allocator<W>> class Container = vector>
class myclass {
public:
    myclass()
    {
        for (int i = 0; i < 10; i++) {
            myc.push_back(i);
        }
    }

    void func();

public:
    Container<T> myc;
};

int main(void)
{
    myclass<double, list> myObj;
}