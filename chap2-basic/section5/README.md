# chap5 - 多态在模板中的应用

回顾一下 面向对象 的多态

1. 继承，父类中有虚函数，子类重写父类的虚函数
2. 父类指针指向子类对象
3. 父类指针调用 虚函数

模板中的多态，并不需要用到父类以及继承的概念、也不需要虚函数

```cxx
class Men {
public:
    void eat()
    {
        cout << "男人喜欢吃面食" << endl;
    }
};

class Women {
public:
    void eat()
    {
        cout << "女人喜欢吃米饭" << endl;
    }
};

/**
 * @brief 函数模板
 *
 * @tparam T
 * @param obj
 */
template <typename T>
void eatTmp1(T& obj)
{
    obj.eat();
}

int main(void)
{
    Men objMen;
    Women objWomen;

    /* 这里自动推断了 */
    eatTmp1(objMen);
    eatTmp1(objWomen);
}
```

实际上，这里的多态是：静态多态，是在编译期的（自动推断）

总结：

- 传统多态：动态多态（运行时多态），因为要访问虚函数表指针，所以对性能多少会有一些影响
- 模板多态：静态多态，编译期间就确定了具体调用谁，就不存在执行期间的性能问题
  只要支持了相同的语法，也就是这里的`obj.eat()`，就允许不同类型的对象以同样的方式被操纵，
  那么就允许调用该成员函数。
