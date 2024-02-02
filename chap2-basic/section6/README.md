# chap6 - 模板的一些特殊继承关系说

## 1 - 奇异（奇特）的递归模板模式（curiously recurring template pattern, CRTP）

是一种模板编程手法：把派生类作为基类的模板参数。

```cxx
/**
 * @brief Base 是类模板
 *
 * @tparam T
 */
template <typename T>
class Base { };

/**
 * @brief 普通的类
 *
 */
class Derived1 : public Base<Derived1> {
public:
    void func()
    {
        cout << "Derived::func() 执行了" << endl;
    }
};

/**
 * @brief 类模板
 *
 * @tparam T
 */
template <typename T>
class Derived2 : public Base<Derived2<T>> { };
```

### 在基类中使用派生类对象

防止 笔误 之类的 隐晦错误发生

```cxx
template <typename T>
class Base {
public:
    void asDerived()
    {
        /**
         * @brief 把自己转换为 派生类对象，
         * 		在单继承下，父类对象与派生类对象首地址对齐的
         *
         */
        T& derived = static_cast<T&>(*this);
        derived.func(); /* 调用派生类对象的成员函数 */
    }

private:
    /**
     * @brief 解决笔误的问题：私有化构造函数、派生类作为友元。这就是为了防止写错
     *
     */
    Base() { }
    friend T;
};

class Derived3 : public Base<Derived1> {
public:
};

int main(void)
{
    /* 错误： Default constructor of 'Derived3' is implicitly deleted
                because base class 'Base<Derived1>' has an inaccessible default constructor */
    Derived3 myd3;
}
```

### 基于减少派生类中代码量的考虑

出发点是：尽可能把一些代码挪到基类中，从而有效的减少派生类中的代码量。
（确实啊，包括设计模式啥的，都是将尽可能多的代码放到基类中，只是声明了一些接口让子类实现）

```cxx
template <typename T>
struct shape {
    /**
     * @brief 在类模板中定义友元函数，相当于是定义全局函数
     *
     */
    friend bool operator==(const shape<T>& obj1, const shape<T>& obj2)
    {
        const T& objtmp1 = static_cast<const T&>(obj1);
        const T& objtmp2 = static_cast<const T&>(obj2);

        if (objtmp1 < objtmp2 == false && objtmp2 < objtmp1 == false) {
            return true;
        } else {
            return false;
        }
    }
};

/**
 * @brief 正方形
 *
 */
struct square : public shape<square> {
    int sidelen;
};

/**
 * @brief 类外重载运算符
 *
 */
bool operator<(square const& obj1, square const& obj2)
{
    if (obj1.sidelen < obj2.sidelen) {
        return true;
    } else {
        return false;
    }
}

int main(void)
{
    square sq1;
    square sq2;

    sq1.sidelen = 15;
    sq2.sidelen = 15;

    cout << (sq1 == sq2) << endl;
}
```

### 基类调用派生类的接口 与 多态的体现

```cxx
template <typename T>
class Human {
public:
    T& toChild()
    {
        return static_cast<T&>(*this);
    }

    void parentEat()
    {
        toChild().eat();
    }

private:
    Human() { }
    friend T;
};

class Men : public Human<Men> {
public:
    void eat()
    {
        cout << "男人喜欢吃面食" << endl;
    }
};

class Women : public Human<Women> {
public:
    void eat()
    {
        cout << "女人喜欢吃米饭" << endl;
    }
};

template <typename T>
void myHumanFuncTest(Human<T>& tmpobj)
{
    tmpobj.parentEat();
}

int main(void)
{
    Men menObj;
    Women womenObj;

    menObj.parentEat(); // 男人喜欢吃面食
    womenObj.parentEat(); // 女人喜欢吃米饭

    cout << "----------  ----------" << endl;

    myHumanFuncTest(menObj); // 男人喜欢吃面食
    myHumanFuncTest(womenObj); // 女人喜欢吃米饭
}
```

这个比较有意思：

```cxx
class Human {
public:
	void parentEat()
    {
        toChild().eat();
    }
};
```

这里是 派生类中有 eat 这个函数，给基类提供接口。
设计模式中，都是基类声明接口，然后子类实现接口。
这里的 “静态多态”，有意思有意思。

静态多态：父类 与 子类并没有同名函数

## 2 - 混入（mixins）

混入：是一种编程手法，用于描述类与类之间的一种关系。这种关系类似于多重继承，
看起来更像是颠倒过来的继承（有点派生类继承基类的感觉）。

还如的实现手段：把传入的模板参数当做该类模板的父类。

### 常规范例

引入混入手段取代传统的继承，这种混入手段看起来更像是：把某个或者某些类混合到当前类中，凑成一个更大的类。

```cxx
template <typename... T>
class role : public T... {
public:
    role()
        : T()...
        , m_attack(0.0)
        , m_defence(0.0)
        , m_life(100.0)
    {
    }

    role(double att, double def, double life)
        : T()...
        , m_attack(att)
        , m_defence(def)
        , m_life(life)
    {
    }

public:
    double m_attack;
    double m_defence;
    double m_life;
};

struct NPCAttr {
    int m_sort;
    string m_lang; /* 自言自语的一句话 */
};

using role_npc = role<NPCAttr>;
```

这个`using role_npc = role<NPCAttr>;`的效果类似于：
`class role_npc : public role, public npcattr { };`（多重继承）

接下来就要调整 family 类了，因为这里的 T 应该是 可变参模板

```cxx
template <typename T>
class family {
public:
    vector<T> m_members;
    /* 其他信息 */
};
```

改成：

```cxx
template <typename... T>
class family {
public:
    vector<role<T...>> m_members;
};

using family_npc = family<NPCAttr>;

int main(void) {
	family_npc mynpcf;
    mynpcf.m_members.push_back(myNPC);
}
```

### 用参数化的方式表达成员函数的虚拟性

是一种设计理念，并不是一种语言特性。体现了一种开发智慧

```cxx
template <typename... T>
class Base : public T... /* 这种 继承 T... 被称为混入 */ {
public:
    /**
     * 主要注意的问题是：如果 T... 中的某个类，包含 virtual myfunc，
     * 		那么显然，这里的 Base::myfunc() 就自动变成 virtual，
     *		并且，如果父类指针指向子类对象，那么这个问题就有趣了，
     *		可能会导致
     *
     */
    void myfunc()
    {
        cout << "Base::myfunc() 执行了!" << endl;
    }
};

template <typename... T>
class Derived : public Base<T...> {
public:
    void myfunc()
    {
        cout << "Derived::myfunc() 执行了!" << endl;
    }
};

class A {
};

class VirA {
    virtual void myfunc()
    {
    }
};

int main(void)
{
    Base<A>* pb1 = new Derived<A>; /* 父类指针指向子类对象 */
    pb1->myfunc(); // Base::myfunc() 执行了!

    /* 这里通过指定不同的参数，看是不是要虚拟化 */

    Base<VirA>* pb2 = new Derived<VirA>; /* 父类指针指向子类对象 */
    pb2->myfunc(); // Derived::myfunc() 执行了!
}
```

一般来说，如果一个类有父类，那么他应该是有析构函数的，并且这个析构函数都应该是一个虚析构。

什么情况下：父类可以没有析构函数 或者 析构函数可以不为虚函数？

1. 子类并没有析构函数（不需要在析构函数中释放任何 new 出来的资源）
2. 代码中不会出现：父类指针指向子类对象（如果是自己写代码，可以规定；或者是在文档中说明；但是违背了一条原则：代码易于正确使用）

有没有什么方式可以防止：父类指针指向子类对象呢？可以把父类的析构函数用 protected 来修饰

有意思的：boost 库中：noncopyable 类
