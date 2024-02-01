/**
 * @file s4_8.cxx 模板模板参数包
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-31
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <cstdio>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

#ifdef gpt

/**
 * @brief 泛化版本的 模板模板参数
 *
 * @tparam T
 * @tparam Container...
 */
template <typename T,
    template <typename> typename... Container // 一包
    >
class ParentMM {
public:
    ParentMM()
    {
        printf("ParentMM::ParentMM() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 偏特化
 *
 * @tparam T
 * @tparam FirstContainers
 * @tparam OthersContainers...
 */
template <typename T,
    template <typename> typename FirstContainers, // 一个
    template <typename> typename... OthersContainers // 一包
    >
class ParentMM<T, FirstContainers, OthersContainers...>
    : private ParentMM<T, OthersContainers...> // 继承
{
public:
    ParentMM()
        : m_containers(vector<T>())
    {
        printf("ParentMM::ParentMM() 偏特化版本执行了, this=%p, sizeof...(Others)=%lu\n", this, sizeof...(OthersContainers));
        m_containers.push_back(12);
    }

    FirstContainers<T> m_containers;
};

template <typename T,
    template <typename> typename... Container // 一包
    >
class myClassT : public ParentMM<T, Container...> {
public:
    myClassT()
    {
        /* boost 中的 type_id_with_cvr<...>().pretty_name() 甚至可以显示万能引用的类型 */
        printf("myClassT::myClassT() 执行了, this=%p, T的类型是: %s, Container 参数个数是 %d 个\n",
            this, typeid(T).name(), sizeof...(Container));
    }
};

int main(void)
{
    myClassT<int, vector, list, deque> myc3;
}

#else

/**
 * @brief 泛化版本的 模板模板参数
 *
 * @tparam T
 * @tparam Container...
 */
template <typename T,
    template <typename, typename...> typename... Container // 使Container能接受至少两个参数
    >
class ParentMM {
public:
    ParentMM()
    {
        printf("ParentMM::ParentMM() 泛化版本执行了, this=%p\n", this);
    }
};

/**
 * @brief 偏特化
 *
 * @tparam T
 * @tparam FirstContainer
 * @tparam OthersContainers...
 */
template <typename T,
    template <typename, typename...> typename FirstContainer, // 一个
    template <typename, typename...> typename... OthersContainers // 一包
    >
class ParentMM<T, FirstContainer, OthersContainers...> : private ParentMM<T, OthersContainers...> // 继承
{
public:
    ParentMM()
        : m_container(std::vector<T>())
    {
        printf("ParentMM::ParentMM() 偏特化版本执行了, this=%p, sizeof...(OthersContainers)=%lu\n", this, sizeof...(OthersContainers));
        m_container.push_back(12);
    }

    FirstContainer<T> m_container; // 注意这里的命名改成了单数形式
};

template <typename T,
    template <typename, typename...> typename... Container // 一包
    >
class myClassT : public ParentMM<T, Container...> {
public:
    myClassT()
    {
        /* boost 中的 type_id_with_cvr<...>().pretty_name() 甚至可以显示万能引用的类型 */
        printf("myClassT::myClassT() 执行了, this=%p, T的类型是: %s, Container 参数个数是 %d 个\n",
            this, typeid(T).name(), static_cast<int>(sizeof...(Container)));
    }
};

int main(void)
{
    myClassT<int, std::vector, std::list, std::deque> myc3;
}

#endif
