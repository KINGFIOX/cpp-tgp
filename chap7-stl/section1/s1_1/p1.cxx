#include <iostream>

// /* ---------- ---------- 打印名字 ---------- ---------- */

// #include <boost/type_index.hpp>

// #define PrintVarType(var)                     \
//     using boost::typeindex::type_id_with_cvr; \
//     std::cout << type_id_with_cvr<decltype(var)>().pretty_name() << std::endl;

// #define PrintType(T)                          \
//     using boost::typeindex::type_id_with_cvr; \
//     std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;

/* ---------- ---------- list ---------- ---------- */

template <typename T>
struct list_node {
    list_node<T>* next;
    list_node<T>* prev;
    T data;
};

template <typename T>
struct list_iterator {
    /* 为防止隐式转换导致 隐含错误，构造函数全部用 explicit 修饰 */
    explicit list_iterator()
        : node(nullptr)
    {
    }

    explicit list_iterator(list_node<T>* tmpNode)
        : node(tmpNode)
    {
    }

    T& operator*()
    {
        return node->data;
    }

    T& operator++() // 重载 前置 ++， ++i
    {
        node = node->next;
        // 前置 ++ 好，不产生临时对象
        return *this; // 编译器不会把这个 * 解析成 operator*
    }

    T& operator++(int) // 重载 后置 ++， i++
    {
        list_node<T> tmp(*this);
        node = node->next;
        return tmp;
    }

    list_node<T>* node; // 定义指向容器节点的 指针
};

template <typename T>
class list {
};

int main(void)
{
}