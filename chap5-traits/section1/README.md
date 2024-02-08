# chap5_1 - 萃取技术 及 fixed traits（固定萃取）演示

traits（萃取）技术用于对模板中的各种模板参数进行管理。
traits 是 特征的意思，但是我们这里翻译成了 萃取

type traits: 类型萃取，实现了许多的类型信息的提取

学习目的：

1. 了解标准库中许多 trait 技术的实现方法
2. 灵活运用并结合这些实现方法，写出功能 更强大、更优雅和使用的代码

fixed traits：给进来一种类型，萃取出另一种类型

## 1 - 一个常规范例

```cxx
/**
 * @brief 计算和值的 函数模板
 */
template <typename U, typename T>
U funcSum(const T* begin, const T* end)
{
    U sum {};
    for (;;) {
        sum += (*begin);
        if (begin == end) {
            break;
        }
        ++begin;
    }
    return sum;
}

int main(void)
{
    int arr1[] = { 10, 15, 20 };
    int arr2[] = { 1000000000, 1500000000, 2000000000 };
    char arr3[] = "abc";

    cout << funcSum<int64_t>(&arr1[0], &arr1[2]) << endl;
    cout << funcSum<int64_t>(&arr2[0], &arr2[2]) << endl;
    cout << funcSum<int64_t>(&arr3[0], &arr3[2]) << endl;
}
```

这里不方便的地方就是：需要手动指定`int64_t`，那有没有更好的解决方案呢？
采用 固定萃取：看 `s5_1/p2.cxx`

## 2 - 迭代起范例

5 类迭代起：

1. 输出型迭代器：`struct output_iterator_tag`
2. 输入型迭代器：`struct input_iterator_tag`
3. 前向迭代器：`struct forward_iterator_tag`
4. 双向迭代器：`struct bidirectional_iterator_tag`
5. 随机访问迭代器：`struct random_access_iterator_tag`

- vector 容器：随机访问迭代器
- list 容器：双向迭代器

本节帮助大家了解：如何通过 固定萃取技术 从容器中萃取出对应的迭代器种类

list 容器对应的迭代器，是一个类或者类模板

```cxx
class _List_iterator{
public:
    using iterator_category = bidirectional_iterator_tag; // 代表 list 容器的迭代器属于哪一个种类的 一个别名类型
    // ...
};

template <class T>
class list{
public:
    using iterator = _List_iterator;
};
```

不难想象：

`list<int>::iterator::iterator_category == bidirectional_iterator_tag`，
这里的`list<int>::iterator`就是`_List_iterator`

1. 在容器中，标示出：所属迭代器的种类
2. fixed trait 类模板，是通过 iterator_traits 实现的

```cxx
void _display_category(std::random_access_iterator_tag mytag)
{
    std::cout << "random_access_iterator_tag" << std::endl;
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
    /* random_access_iterator_tag */
}
```

1. `display_category(vector<int>::iterator())` 创建了一个临时的 `_Vector_iterator` 类对象
2. `display_category`中的代码行`typename IteratorTraits<T>::iterator_category cagy;`等价于
   `typename IteratorTraits<_Vector_iterator>::iterator_category cagy;`
3. iterator_traits 类模板定义中 `IteratorTraits<T>::iterator_category = T::iterator_category`；
   代码行等价于`_Vector_iterator::iterator_category`
4. `_Vector_iterator::iterator_category = random_access_iterator_tag`。通过这个类型创建了`cagy`对象
5. 这个对象传入到对应的`_display_category`，有种多态的感觉

## 3 - 通过容器（数组）类型萃取元素类型范例

### 用 GetEleType 类模板进行常规实现

```cxx
/**
 * @brief 泛化版本不实现
 *
 * @tparam T
 */
template <typename T>
struct GetEleType;

template <typename T>
struct GetEleType<std::vector<T>> {
    using type = T;
};

/**
 * @brief 数组
 * （这个依然是：参数范围上的 偏特化）
 * @tparam T
 * @tparam Size 看起来这个特化版本多了一个 模板参数（但并非是 参数数量上的偏特化）
 */
template <typename T, std::size_t Size>
struct GetEleType<T[Size]> {
    using type = T;
    static const std::size_t size = Size;
};
```

偏特化怎么填写？就是把泛化版本规定的参数给填写完成`template <typename T> struct GetEleType;`。
先确定如何填写 泛化版本，再反推泛化版本的 参数有哪些

## 4 - 引用类型的移除和增加

### 移除

C++标准库中提供了 `std::remove_reference`类模板

### 增加

## 5 - const 修饰符的移除

## 6 - 退化（decay）技术
