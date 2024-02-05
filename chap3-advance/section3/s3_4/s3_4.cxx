/**
 * @file s3_4.cxx 未改进之前的
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-05
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <string>

using namespace std;

class Human {
public:
    /* 构造函数 */
    Human(const string& tmpName)
        : m_SName(tmpName)
    {
        cout << "Human(const string& tmpName) 执行了" << endl;
    }

    // Human(string&& tmpname) : m_SName(tmpname)
    Human(string&& tmpname)
        : m_SName(std::move(tmpname)) /* move 只是将左值变成一个右值，并不具有移动能力 */
    /* 这里就是会执行 string 的移动构造函数（接受右值） */
    /* 将 tmpName 移走，这个不是 std::move 的行为，而是 string 的移动构造函数做的 */
    /* 这个时候 tmpName 就空了 */
    {
        cout << "Human(string&& tmpName) 执行了" << endl;
    }

private:
    string m_SName;
};

int main(void)
{
    string sName = "ZhangSan";
    Human MyHuman1(sName); // Human(const string& tmpName) 执行了
    Human MyHuman2(string("LiSi")); // Human(string&& tmpName) 执行了
}