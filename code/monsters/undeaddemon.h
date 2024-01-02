#ifndef UNDEADDEMON_H
#define UNDEADDEMON_H

#include "stdafx.h"
#include "../monster.h"

class Undead_Demon : public Monster
{
public:
    Undead_Demon(Game* G);
    ~Undead_Demon() override {}
    void reset() override;
    QString stat_string() override;
    GT::PoolType pooltype() override{return GT::POOL_UBDEAD_DEMON;} // 卡牌类型
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_After_Combat() override;
private:
    void clear_piece();

    int rescount;
};

#endif // UNDEADDEMON_H
