#include <boost/type_index.hpp>
#include <iostream>

using namespace std;

template <typename T>
void myfunc(const T& tmprv)
{
    cout << "---------- begin ----------" << endl;

    using boost::typeindex::type_id_with_cvr;
    cout << "T=" << type_id_with_cvr<T>().pretty_name() << endl;
    cout << "tmprv=" << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; /* */

    cout << "---------- end ----------" << endl;
}

int main(void)
{
    myfunc(100);
    /**
     * template <typename T> void myfunc(const T& tmprv) 可以通过
     * 但是 template <typename T> void myfunc(T& tmprv) 不能通过
     * 输出：
     * T=int
     * tmprv=int const&
     */
    return 0;
}
