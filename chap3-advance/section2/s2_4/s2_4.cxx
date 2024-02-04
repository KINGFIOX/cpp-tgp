#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

void test_func(int, int) { }

template <typename T>
void func(T&& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

int main(void)
{
    auto x = 27; /* int */
    const auto x2 = x;
    const auto& xy = x;
    auto xy2 = xy; /* 这个相当于是 传值方式：引用--；const-- */

    auto& xy3 = xy; /* const int & ，多余的 & 会被丢弃 */
    auto y = new auto(100); /* int *，形参是指针 */
    auto* xp2 = &x; /* int *；auto = int */
    auto xp3 = &x; /* int *；auto = int * */

    auto&& wnyy0 = 222; /* int && */
    auto&& wnyy1 = x; /* int & */
    auto&& wnyy2 = x2; /* const int & */
}