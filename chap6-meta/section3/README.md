# chap6_3 - typelist 实现（类型计算的 范例）

有本书叫做《modern C++ design》，这本书 比较 魔怔（来自知乎）。
loki 实现了 typelist（列表头 + 嵌套列表技术来实现，因为 2003 年 C++ 不支持可变参模板）

typelist：用来操作一大堆类型的 C++ 容器，就像 C++ 标准库中的 list 容器
（能够对各种数值提供各种基本操作），但 typelist 是针对类型进行操作。

typelist 简单来说就是一个类型容器，能够对类型提供一系列的操作（把类型当做数据来操作）。

typelist 是一个类模板，类型列表。该类模板用来表示一个列表，在该列表中存放着一堆类型。

## 1 - 设计 和 基本操作接口（算法）

```cxx
template <typename... T>
class typelist { };

int main(void)
{
    typelist<int, double, char, bool> s;
    // 这个容器可以用了，我们接下来就是看看怎么操作这些元素
}
```

### 相关常规操作

front（获取第一个元素，非破坏性）、size、pop_front、push_front、
push_back、replace_front、is_empty

参考 s3_1/p1.cxx。

## 2 - 扩展操作接口（算法）

### 根据索引号 查询 typelist 中的某个元素（find）

看 s3_1/p2.cxx

递归继承是一种编程手法，其实也不一定要递归继承，可以使用 `using type = `，
这其实也相当于是一种继承，只不过只继承了一个接口。

```cxx
/**
 * @brief 泛化版本，每次 pop_front，然后 索引号 --
 *		这里采用递归继承
 * @tparam TPLT
 */
template <typename TPLT, std::size_t index_v>
class find : public find<typename pop_front<TPLT>::type, index_v - 1> { };

/**
 * @brief 特化版本，我们一开始是写：特化版本的
 *
 * @tparam TPLT
 */
template <typename TPLT>
class find<TPLT, 0> : public front<TPLT> { };
```

### 遍历 typelist 找到 sieof 值最大的元素（get_maxsize_type）

复习一下：元编程 计算完整性：

1. 迭代构造（循环构造）：也就是在 `get_maxsize_type` 模板中融入 递归编程技术
2. 状态变量：指的是类模板 `get_maxsize_type` 中的模板参数。
   因为这里类模板的模板参数是 typelist，所以每次递归，typelist 中的元素元素数量肯定要少一个，
   这样：最终才能符合 递归结束条件
3. 执行路径的选择：使用 `std::conditional` 以及 `get_maxsize_type` 类模板的特化达到结束递归的效果，
   这个特化版本的特点是 模板参数 typelist 中的元素为 0 个
4. 运算对象 L 类模板中的 type 成员记录的就是 当前 sizeof 值最大的类型。

```cxx
/**
 * @brief 泛化版本
 *
 */
template <typename TPLT>
class get_maxsize_type {
private:
    using tl_first_elem = typename front<TPLT>::type; // 获取第一个元素
    using tl_remain = typename pop_front<TPLT>::type; // 去掉一个元素

    // 递归下去的 get_maxsize_type
    using tl_first_elem_rec = typename get_maxsize_type<tl_remain>::type;

public:
    using type = typename std::conditional<
        (sizeof(tl_first_elem) > sizeof(tl_first_elem_rec)),
        tl_first_elem,
        tl_first_elem_rec>::type;
};

/**
 * @brief 特化版本
 *
 */
template <>
class get_maxsize_type<typelist<>> {
public:
    using type = char[0]; // 长度为 0 的数组
};

int main(void) {
    // using TPL_NM1 = typelist<int, double, char, bool>;
    // max_type<typelist<int, double, char, bool>>::type
    // -> first = int；rec = max_type<typelist<double, char, bool>>::type -> max
    // // max_type<typelist<double, char, bool>>::type
    // // -> first = double；rec = max_type<typelist<char, bool>>::type -> max
    // // // max_type<typelist<char, bool>>::type
    // // // -> first = char；rec = max_type<typelist<bool>>::type -> max
    // // // // max_type<typelist<bool>>::type
    // // // // -> first = bool；rec = max_type<typelist<>>::type -> max
    // // // // // max_type<typelist<>>::type = char[0]
    // // // // max(bool, char[0]) -> bool -> max_type<typelist<bool>>::type = bool
    // // // ... 向上传递
}
```

### 反转一个 typelist 中的元素顺序（reverse）

```cxx
/* ---------- ---------- 反转 reverse ---------- ---------- */

/**
 * @brief 泛化版本
 *
 * @tparam TPLT
 * @tparam bool
 */
template <typename TPLT, bool = is_empty<TPLT>::value>
class reverse;

/**
 * @brief 非空的时候（特化版本）
 *
 * @tparam TPLT
 */
template <typename TPLT>
class reverse<TPLT, false> {
private:
    using _rid_front = typename pop_front<TPLT>::type;

    using tl_first_elem = typename front<TPLT>::type;
    using tl_result_rec = typename reverse<
        _rid_front, is_empty<_rid_front>::value>::type;
    // 把 front 插入尾部

public:
    using type = typename push_back<tl_result_rec, tl_first_elem>::type;
    // push 递归的 using。
};

/**
 * @brief 空的时候（特化版本）
 *
 * @tparam TPLT
 */
template <typename TPLT>
class reverse<TPLT, true> {
public:
    using type = TPLT; /* 实际上也就是 typelist<> */
};

int main(void)
{
    std::cout << typeName<reverse<TPL_NM1>::type>() << std::endl; // double
    // using TPL_NM1 = typelist<int, double, char, bool>;
    // reverse<TL<int, double, char, bool>>
    // -> first = int, rec = reverse<double, char, bool> -> pb<rec, first>
    // // reverse<double, char, bool>
    // // -> first = double, rec = reverse<char, bool> -> pb<rec, first>
    // // // reverse<char, bool>
    // // // -> first = char, rec = reverse<bool> -> pb<rec, first>
    // // // // reverse<bool>
    // // // // -> first = bool, rec = TL<> -> pb<rec, int> = TL<bool>
    // // // reverse<bool> = TL<bool> -> pb< rev, char > -> TL<bool, char>
    // // reverse<bool, char> = TL<bool, char> -> pb< rev, double > -> TL<bool, char, double>
    // reverse<bool, char, double> = TL<bool, char, double>
    // -> pb< rev, int> -> TL<bool, char, double, int>
}
```

## 3 - typelist 的老式设计 与 typelist 的思考

### 老式设计

也就是《modern C++ design》的实现方法（loki 库），
没有可变参函数模板

### 思考

这个老式的 typelist 相当于是变向的提供了：可变参模板，
这在当时还是比较有意思的

typelist 比较适合于：一次传递很多个类型的场合
