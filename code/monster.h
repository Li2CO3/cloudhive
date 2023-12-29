#ifndef MONSTERS_H
#define MONSTERS_H

#include <QString>
#include <QDialog>

#include "stdafx.h"
#include "utils/gametools.h"

#define POOLTYPE gametools::PoolType
#define RULE gametools::SCORING_RULE


class Game;

class Monster
{
public:

    Game* G;
    QString name;
    QString shortname;
    QString icon;
    GT::MONSTER_ID id;
    int initialhealth;//在构造的时候就设置上了
    int point;
    int health;
    int armor;
    bool dead;
    Monster(Game* g);
    virtual ~Monster();

    virtual QString stat_string();
    static Monster* new_monster(GT::MONSTER_ID monst,
                                Game* G);//把Game的Monster替换成一个新的，根据monst创建

    virtual QString description();//描述
    virtual void reset() { health = initialhealth; armor = 0; point = 0; }
    virtual void setStatus() ;
    virtual GT::PoolType pooltype() { return GT::POOL_Normal; }//怪对应的牌堆类型
    virtual GT::SCORING_RULE scoringrule() { return GT::Scoring_Normal; }//计分规则。
    virtual void Monster_Before_Turn();//包含发牌
    virtual void Monster_Before_Combat();//发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player->prev_point]
    virtual void Monster_Combat();//打架效果，包含掉血
    virtual void Monster_After_Combat();//打架后效果
    virtual void addPoint(int pt);
    void gainArmor(int arm);
    virtual void deal_damage(int dmg);
    virtual void take_damage(int dmg);
    virtual void Make_Summary(QDialog* dialog);

};
#endif // MONSTERS_H
