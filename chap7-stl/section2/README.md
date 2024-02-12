# chap7_2 - 迭代器分类、find_if 算法的简单实现

## 1 - 迭代器分类

### 迭代器的分类

5 种 迭代器：

1. output
2. input
3. forward
4. bidirectory
5. random access

```cxx
int main(void) {
    cout << typeid(list_iterator<int>::iterator_category).name() << endl;
}
```

### 迭代器分类目的详述

与 算法 有直接关系（算法的策略）

需要对 iterator_category 的常用算法，有义务的实现（如果是写 STL 容器的话）

## 2 - 一个典型算法 —— find_if 的简单实现

迭代器将：算法 和 数据存储 进行了分离

```cxx
template <typename T, typename U>
T FindIf(T first, T last, U funcObj)
{
    for (; first != last; ++first) {
        if (funcObj(*first)) {
            break;
        }
    }
    return first;
}
```
