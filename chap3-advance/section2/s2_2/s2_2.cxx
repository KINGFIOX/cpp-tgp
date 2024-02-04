#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

template <typename T>
void myfunc1(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;

    tmprv = 12;
}

template <typename T>
void myfunc2(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;

    tmprv = 18;
}

template <typename T>
void myfunc3(T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

template <typename T>
void myfunc4(T tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

template <typename T>
void myfunc5(const T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

template <typename T>
void myfunc6(T* tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void)
{
    int i = 1;
    // const int j = i;
    // const int& k = i;
    // myfunc(i);
    // /**
    //  * T = int
    //  * tmprv = int &
    //  */
    // myfunc(j);
    // /**
    //  * T=int const
    //  * tmprv=int const&
    //  */
    // myfunc(k);
    // /**
    //  * T=int const
    //  * tmprv=int const&
    //  */

    // myfunc1(i);
    // cout << i << endl; // 12

    int& j = i;
    myfunc2(j);
    /**
     * T=int
     * tmprv=int
     */
    cout << j << endl; // 1

    const int l = i;
    const int& k = i;

    myfunc3(k);
    /**
     * T=int const
     * tmprv=int const&
     */
    myfunc4(k);
    /**
     * T=int
     * tmprv=int
     */

    myfunc5(i);
    /**
     * T=int
     * tmprv=int const&
     */
    myfunc5(k);
    /**
     * T=int
     * tmprv=int const&
     */

    myfunc5(l);
    /**
     * T=int
     * tmprv=int const&
     *
     */

    const int* pi = &i;
    myfunc6(&i);
    /**
     * T=int
     * tmprv=int*
     */

    myfunc6(pi);
    /**
     * T=int const
     * tmprv=int const*
     */

    return 0;
}
