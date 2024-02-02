/**
 * @file s6_5.cxx 采用混入手段实现 role_player 、 role_npc
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-02-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <iostream>
#include <vector>

using namespace std;

/**
 * role 角色类，代表玩家，包括 攻击力、防御力、血量
 * 		大型多人网游，还有家族啥的
 *
 */

/**
 * @brief 定义出来了 可变参类模板
 *
 * @tparam T
 */
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

// 但是 NPC 实际上有很多类：0. 装饰 1. 商人 2. 分派任务。
// NPC 自言自语的说话
struct NPCAttr {
    int m_sort;
    string m_lang; /* 自言自语的一句话 */
};

// // NPC 类
// class role_npc : public role {
// public:
//     role_npc()
//         : role()
//         , m_structAtt { 0, "" }
//     {
//     }

//     role_npc(double att, double def, double life, int sort, string lang)
//         : role(att, def, life)
//         , m_structAtt { sort, lang }
//     {
//     }

// public:
//     NPCAttr m_structAtt;
// };

/**
 * @brief 这个就代表了之前写的 role_npc 了
 *
 */
using role_npc = role<NPCAttr>;

// 后期引入 玩家属性系统，分为3种：力量、敏捷、体质。
// 玩家每升一级，就能得到10个属性点，可以把属性点加到这三中属性上去
// 最终目的就是：提高玩家的攻击力、防御力、血量
struct playerAttr {
    int m_strength; /* 力量 */
    int m_agile; /* 敏捷 */
    int m_constitution; /* 体质 */
};

using role_player = role<playerAttr>;

using role_mixnpc = role<NPCAttr, playerAttr>; /* 混入 */

// /**
//  * @brief 玩家类
//  *
//  */
// class role_player : public role {
// public:
//     role_player()
//         : role()
//         , m_structAtt { 0, 0, 0 }
//     {
//     }

//     role_player(double att, double def, double life, int s, int a, int c)
//         : role(att, def, life)
//         , m_structAtt { s, a, c }
//     {
//     }

// public:
//     playerAttr m_structAtt;
// };

/*
        上面这段代码的问题，有参构造很长，并且代码的可拓展性差，
        如果有新的 role 字段，那么修改是很大的，非常的不满足开闭原则
                我们可以引入 混入的手段，取代传统的继承。
*/

/* ---------- ---------- 下面是家族系统 ---------- ----------  */

// class family {
// public:
//     vector<role> m_members;
// };
// 随着游戏的发展，这个 family 以后不只是 玩家的，还有可能是 NPC 的
// 那么这个 family 或许要写成 泛型的，因此 vector<role> 写死了就不太好。

// template <typename T>
// class family {
// public:
//     vector<T> m_members;
//     /* 其他信息 */
// };

template <typename... T>
class family {
public:
    vector<role<T...>> m_members;
};

using family_npc = family<NPCAttr>;

int main(void)
{
    role_npc myNPC;
    myNPC.m_attack = 15;
    myNPC.m_defence = 10;
    myNPC.m_life = 120;
    myNPC.m_sort = 1;
    myNPC.m_lang = "Are you ok?";

    family_npc mynpcf;
    mynpcf.m_members.push_back(myNPC);
}