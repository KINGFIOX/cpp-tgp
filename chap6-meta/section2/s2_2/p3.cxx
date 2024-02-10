#include <iostream>
#include <type_traits>

// struct TST {
//     static const int myStatic = 1;
//     /* 但是这种初始化方式有点问题：只能是 int 相关的类型：short、long、char */
//     /* 其次必须用 const 修饰 */
//     /* 但是其实也是有问题的，这样操作的后果：静态成员变量的生命周期是 覆盖整个程序运行时 */
// };

// struct TST {
//     static inline double myStatic = 1;
//     /* 如果在代码中没有用到这个类，这个静态成员 就不会初始化 */
// };

struct TST {
    // static inline double myStatic = 1;
    static constexpr double myStatic = 1;
    /* 如果在代码中没有用到这个类，这个静态成员 就不会初始化 */
};

int main(void)
{
}