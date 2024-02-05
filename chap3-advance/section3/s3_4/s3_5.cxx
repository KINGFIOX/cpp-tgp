/**
 * @file s3_5.cxx 改进之后的
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
#include <utility>

using namespace std;

class Human {
public:
    /**
     * @brief 构造函数模板
     *
     * @tparam T
     * @param tmpName
     */
    template <typename T>
    Human(T&& tmpName)
        : m_SName(std::forward<T>(tmpName)) /* 完美转发 */
    {
        cout << "Human(T&&) 执行了" << endl;
    }

    Human(const Human& th)
        : m_SName(th.m_SName)
    {
        cout << "Human 的拷贝构造函数执行了" << endl;
    }

    Human(Human&& th)
        : m_SName(std::move(th.m_SName))
    {
        cout << "Human 的移动构造函数执行了" << endl;
    }

private:
    string m_SName;
};

int main(void)
{
    string sName = "ZhangSan";
    Human MyHuman1(sName);
    Human MyHuman2(string("LiSi"));

    // Human MyHuman3(MyHuman1); /* 错误：Boom! 主要问题是走进去了 万能引用中。
    //     我们实际上是希望调用拷贝构造，然后不可能将 MyHuman 对象 有参构造 string 对象的，而不是有参构造 */
    /* 我们现在就是想要让编译器去执行 拷贝构造函数，而不是 有参构造模板 ---> std::enable_if */

    Human MyHuman4(std::move(MyHuman1)); /* ✅ */

    const Human MyHuman5(string("Wangwu"));
    Human MyHuman6(MyHuman5); /* ✅ Human 的拷贝构造函数执行了 */
}