/**
 * @file p1.cxx
 * @author your name (you@domain.com)
 * @brief 是否可拷贝的 直观感受
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <type_traits>

using namespace std;

class ACPABL { };

class BCPABL {
public:
    BCPABL& operator=(const BCPABL& tmpObj)
    {
        return *this;
    }
};

class CCPABL {
public:
    /* 标记为 delete 或者是 私有，那么就不能被拷贝 */
    BCPABL& operator=(const CCPABL& tmpObj) = delete;
};

int main(void)
{
    BCPABL bObj1;
    BCPABL bObj2;

    cout << std::is_copy_assignable<int>::value << endl; // 1
    cout << std::is_copy_assignable<ACPABL>::value << endl; // 1
    cout << std::is_copy_assignable<BCPABL>::value << endl; // 1
    cout << std::is_copy_assignable<CCPABL>::value << endl; // 0
}