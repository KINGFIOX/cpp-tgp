#include <iostream>

/* ---------- ---------- 打印名字 ---------- ---------- */

#include <boost/type_index.hpp>

#define PrintVarType(var)                     \
    using boost::typeindex::type_id_with_cvr; \
    std::cout << type_id_with_cvr<decltype(var)>().pretty_name() << std::endl;

#define PrintType(T)                          \
    using boost::typeindex::type_id_with_cvr; \
    std::cout << type_id_with_cvr<T>().pretty_name() << std::endl;

/* ---------- ---------- list ---------- ---------- */

template <typename T>
struct list_node {
    explicit list_node<T>(const T& x)
        : data(x)
        , next(nullptr)
        , prev(nullptr)
    {
    }

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

    /* ---------- 获取元素 ---------- */

    T& operator*()
    {
        return node->data;
    }

    /* ---------- ++ ---------- */

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

    /* ---------- -- ---------- */

    T& operator--() // 重载 前置 ++， ++i
    {
        node = node->prev;
        // 前置 ++ 好，不产生临时对象
        return *this; // 编译器不会把这个 * 解析成 operator*
    }

    T& operator--(int) // 重载 后置 ++， i++
    {
        list_node<T> tmp(*this);
        node = node->prev;
        return tmp;
    }

    /* ---------- 相等 ---------- */

    bool operator!=(const list_iterator<T>& tmpObj)
    {
        return node != tmpObj.node;
    }

    bool operator==(const list_iterator<T>& tmpObj)
    {
        return node == tmpObj.node;
    }

    /* ---------- 数据：指针 ---------- */

    list_node<T>* node; // 定义指向容器节点的 指针
};

template <typename T>
class list {
public:
    using iterator = list_iterator<T>;

public:
    explicit list()
    {
        void* point = (void*)(new char[sizeof(list_node<T>)]);
        head = reinterpret_cast<list_node<T>*>(point); // 硬转
        head->next = head; // 手动构造，没有调用 list_node 的构造函数
        head->prev = head;
        // 把 head->T 空出来了
    }
    ~list()
    {
        delete[] (char*)head;
        head = nullptr;
    }

public:
    iterator begin()
    {
        return iterator(head->next);
        // begin 是 head 这个节点的下一个节点 head->next
    }

    iterator end()
    {
        return iterator(head);
        // end 是 head 这个节点本身（特殊节点，哨兵）
    }

public:
    void push_back(const T& tmpElem)
    {
        list_node<T>* pNewNode = new list_node<T>(tmpElem);

        if (head->next == head) { // 空链表
            pNewNode->next = head;
            pNewNode->prev = head;
            head->next = pNewNode;
            head->prev = pNewNode;
        } else {
            // 当前容器不为空
            pNewNode->next = head;
            pNewNode->prev = head->prev; // head.prev 就是：最后一个节点
            head->prev->next = pNewNode;
            head->prev = pNewNode;
        }
    }

private:
    void clear() // 不会破坏 head 节点
    {
        if (head->next != head) { // 容器非空
            list_node<T>* curNode = head->next;
            while (curNode != head) {
                list_node<T>* nextNode = curNode->next;
                delete curNode;
                curNode = nextNode;
            }
        }
    }

private:
    list_node<T>* head; // 头节点，不存数据。head.prev 是最后一个元素；head.next 是第一个元素
};

/* ---------- ---------- 测试 ---------- ---------- */

class A {
public:
    explicit A(int tmpi)
        : m_i(tmpi)
    {
    }
    int m_i;
};

int main(void)
{
    list<A> t;
    t.push_back(A(2));
    t.push_back(A(1));
    t.push_back(A(3));
}