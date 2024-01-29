#include <cstdio>
#include <iostream>

using namespace std;

template <const char* p>
struct TC {
    TC()
    {
        printf("TC::TC 执行了, p= %s\n", p);
    }
};

// const char* g_s = "hello"; /* 全局指针 */
// int main(void)
// {
//     TC<g_s> myTC; /* 错误： Non-type template argument is not a constant expression */
//     return 0;
// }

const char g_s[] = "hello"; /* 常量数组 */
int main(void)
{
    TC<g_s> myTC;

    // TC<"hello"> myTc2; /* 错误：Pointer to subobject of string literal is not allowed in a template argument */

    return 0;
}
