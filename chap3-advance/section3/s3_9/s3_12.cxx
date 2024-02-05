#include <iostream>
#include <type_traits>

using namespace std;

/* 给类型模板定义一个别名 */
template <typename T>
using StrProcType = std::enable_if_t<std::is_convertible<T, std::string>::value>;

class Human {
public:
    /**
     * @brief 构造函数模板
     *
     * @tparam T
     * @param tmpName
     */
    // template <typename T>
    /* template <typename T,
         typename = std::enable_if_t<std::is_convertible<T, string>::value>> */
    template <typename T, typename = StrProcType<T>>
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

    Human MyHuman3(MyHuman1); // Human 的拷贝构造函数执行了
}