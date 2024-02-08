#include <iostream>
#include <iterator>
#include <vector>

void _display_category(std::random_access_iterator_tag mytag)
{
    std::cout << "random_access_iterator_tag" << std::endl;
}

void _display_category(std::bidirectional_iterator_tag mytag)
{
    std::cout << "bidirectional_iterator_tag" << std::endl;
}

void _display_category(std::forward_iterator_tag mytag)
{
    std::cout << "forward_iterator_tag" << std::endl;
}

void _display_category(std::output_iterator_tag mytag)
{
    std::cout << "output_iterator_tag" << std::endl;
}

void _display_category(std::input_iterator_tag mytag)
{
    std::cout << "input_iterator_tag" << std::endl;
}

template <typename IterT>
struct IteratorTraits {
    using iterator_category = typename IterT::iterator_category;
};

template <typename T>
void display_category(T iter)
{
    // typename std::iterator_traits<T>::iterator_category cagy; // 萃取机，cagy 是对象
    typename IteratorTraits<T>::iterator_category cagy; // 萃取机，cagy 是对象
    _display_category(cagy); /*  */
}

int main(void)
{
    display_category(std::vector<int>::iterator()); // 创建了一个临时的 _Vector_iterator 类对象，
}