#include <boost/type_index.hpp>
#include <initializer_list>
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

template <typename T, unsigned L1>
void func4(T (&tmprv)[L1])
{
    // cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    // cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(T).name() << endl;
    cout << typeid(tmprv).name() << endl;
    cout << "L1=" << L1 << endl;
    /**
     * c
     * A9_c
     * L1=9
     */
}

template <typename T>
void func2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

template <typename T>
void func3(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */
    cout << typeid(tmprv).name() << endl;

    cout << "---------- end ----------" << endl;
}

template <typename T>
void func5(std::initializer_list<T> tmprv)
{
    cout << "T=" << typeid(T).name() << endl;
    cout << "tmprv=" << typeid(tmprv).name() << endl;
}

int main(void)
{

    int i = 18;
    // func(i);
    // /**
    //  * T=int&
    //  * tmprv=int&
    //  *
    //  */

    // int j = i;
    // func(j);
    // /**
    //  * T=int const&
    //  * tmprv=int const&
    //  */

    // const int& k = i;
    // func(k);
    // /**
    //  * T=int const&
    //  * tmprv=int const&
    //  */

    // func(100);
    // /**
    //  * T=int
    //  * tmprv=int&&
    //  */

    // func2(i);
    // /**
    //  * T=int
    //  * tmprv=int
    //  */

    // func2(j);
    // /**
    //  * T=int
    //  * tmprv=int
    //  */

    // func2(k);
    // /**
    //  * T=int
    //  * tmprv=int
    //  */

    // func2<int&>(i);
    // /**
    //  * T=int&
    //  * tmprv=int&
    //  */

    // func2(&i);
    // /**
    //  * T=int*
    //  * tmprv=int*
    //  */

    // char p[] = "fuck";
    // func2(p);
    // /**
    //  * T=char*
    //  * tmprv=char*
    //  */

    // const char* cp = p;
    // func2(cp);
    // /**
    //  * T=char const*
    //  * tmprv=char const*
    //  */

    // char* const cpc = p;
    // func2(cpc);
    // /**
    //  * T=char const*
    //  * tmprv=char const* 第一个 const 会被忽略掉
    //  */
    // cout << typeid(cpc).name() << endl; // Pc

    // int m = 180;
    // func2(m);
    // func2(std::ref(m));
    // /**
    //  * T=std::__1::reference_wrapper<int>
    //  * tmprv=std::__1::reference_wrapper<int>
    //  */

    // const char str[] = "fuck you";
    // func2(str);
    // /**
    //  * T=char const*
    //  * tmprv=char const*
    //  */

    // func3(str);
    // /**
    //  * T=char const (&) [9]
    //  * tmprv=char const (&) [9]
    //  */

    // func4(str);

    func2(test_func);
    /**
     * T=void (*)(int, int)
     * tmprv=void (*)(int, int)
     */

    func3(test_func);
    /**
     * T=void (int, int)
     * tmprv=void (&)(int, int)
     */

    func(test_func);
    /**
     * T=void (&)(int, int)
     * tmprv=void (&)(int, int)
     */

    // func({ 1, 3 }); /* 错误：No matching function for call to 'func' */
    func5({ 12, 3 });
    /**
     * T=i
     * tmprv=St16initializer_listIiE
     */
}
