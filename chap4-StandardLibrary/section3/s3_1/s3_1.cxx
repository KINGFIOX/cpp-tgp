/**
 * @file s3_1.cxx 判断是否有类型别名
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <type_traits>

using namespace std;

struct NoInnerType {
    /* 没有类型别名 */
    int m_i;
};

struct HaveInnerType {
    using type = int; /* 类型别名 */
    void myfunc() { }
};

#define HAS_TYPE_MEM(parMTpNm)                        \
    template <typename T, typename U = std::void_t<>> \
    struct HTM_##parMTpNm : std::false_type { };      \
    template <typename T>                             \
    struct HTM_##parMTpNm<T, std::void_t<typename T::parMTpNm>> : std::true_type { };
/* ##parMTpNm 这里的 ##parMTpNm 解引用 + 字符串连接 */

/* 使用宏定义：HTM_type */
HAS_TYPE_MEM(type);

int main(void)
{
    /* 这个 value 来源于继承 */
    /* 并且我们当前的代码，只能识别 public 修饰的，不能识别 private 修饰的 */
    // cout << HasTypeMem<NoInnerType>::value << endl; // 0
    // cout << HasTypeMem<HaveInnerType>::value << endl; // 1

    cout << HTM_type<HaveInnerType>::value << endl;
}