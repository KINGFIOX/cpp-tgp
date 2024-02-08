#include <boost/type_index.hpp>
#include <iostream>

template <typename T>
void func(T tmpRv)
{
    std::cout << "---------- begin ----------" << std::endl;
    using boost::typeindex::type_id_with_cvr;
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << type_id_with_cvr<decltype(tmpRv)>().pretty_name() << std::endl;
    std::cout << "---------- end ----------" << std::endl;
}

void testFunc()
{
}

int main(void)
{
    const int a = 16;
    const int& b = a;
    const int&& c = 18;

    int arr[5] = { 1, 2, 3, 4, 5 };

    func(a);
    /**
     * int
     * int
     */
    func(b);
    /**
     * int
     * int
     */
    func(c);
    /**
     * int
     * int
     */
    func(arr);
    /**
     * int*
     * int*
     */
    func(testFunc);
    /**
     * void (*)()
     * void (*)()
     */
}