#include "twinhead.h"
#include "game.h"
#include "cardpool.h"

Twin_Head::Twin_Head(Game* G) : Monster(G) {
    id = GT::TWIN_HEAD;
    name = "双头龙"; shortname = "双头龙";
    initialhealth = 2000;
    icon = QString("res/icon/monster/") + "1_twin_head.jpg";
}


QString Twin_Head::description()
{
    QString str = "第一期蜂巢副本BOSS：" + this->name + " 初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能1：【间歇协作】从第三回合起，第奇数回合增长4点战力，第偶数回合增长8点战力。\n\n";
    str += "被动技能2：【守守兼顾】对受到的奇数伤害形成15点格挡，受到的偶数伤害提升20点。\n\n";
    str += "主动技能：【固守】分别在第10、20、30回合结束后进行一次判定：\n";
    str += "·若挑战者战力为奇数，获得(挑战者战力*150%)点护盾，\n";
    str += "·若挑战者战力为偶数，获得(挑战者战力*50%)点护盾。（向下取整）\n\n";
    str += "副本环境·奇数规约：卡池中没有4，每种3和8各有3张。";
    return str;
}

void Twin_Head::Monster_Before_Turn()//双头龙：回合开始
{
    Monster::Monster_Before_Turn();
    if (G->turn >= 3)
    {
        int gain_pt = ((G->turn % 2 == 1) ? 4 : 8);
        emit G->Alert_monster("双头龙【间歇协作】下回合增加" + QN(gain_pt) + "分");
    }
    if (G->turn % 10 == 0)
        emit G->Alert_monster("双头龙【固守】即将触发，获得护盾\n如果得分为奇数获得大量护盾");
}

void Twin_Head::Monster_Before_Combat()//双头龙：回合开始
{
    if (G->turn >= 3)
    {
        int gainpt = (G->turn % 2 == 1 ? 4 : 8);
        emit G->Alert_monster("双头龙【间歇协作】触发，+" + QN(gainpt) + "分!");
        addPoint(gainpt);
    }

    if (G->turn % 10 == 0)
    {
        int pt = G->player()->point();
        int gain_armor = ((pt % 2) ? (pt + pt / 2) : (pt / 2));
        emit G->Alert_monster(QString("双头龙【固守】触发，获得+") + QString::number(gain_armor) + "护盾!");
        gainArmor(gain_armor);
    }
    return;
}

void Twin_Head::Monster_Combat()
{
    int mypt = G->player()->point();
    int monsterpt = G->monster()->point;
    int difference = mypt - monsterpt;
    int damage = difference;
    if (mypt > monsterpt)
    {
        emit G->Alert_monster(G->player()->name + " vs " + name + "(-" + QN(difference) + ")");
        if (mypt % 2 == 1)
        {
            damage = std::max(difference - 15, 0);
            emit G->Alert_monster(QString("奇数格挡，伤害-15(变为") + QN(damage) + ")");
        }
        else if (mypt % 2 == 0)
        {
            damage += 20;
            emit G->Alert_monster(QString("偶数暴击，伤害+20(变为") + QN(damage) + ")");
        }
        take_damage(damage);
    }
    if (mypt == monsterpt || mypt < monsterpt)
    {
        Monster::Monster_Combat();
    }
    return;
}
