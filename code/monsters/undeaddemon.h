#ifndef UNDEADDEMON_H
#define UNDEADDEMON_H

#include "stdafx.h"
#include "../monster.h"

class UndeadDemon : public Monster
{
public:
    UndeadDemon(Game* G);
    ~UndeadDemon() override {}
    void reset() override;
    GT::PoolType pooltype() override{return GT::POOL_UBDEAD_DEMON;} // 卡牌类型
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_After_Combat() override;
private:
    void clear_piece();

    int rescout;
};

#endif // UNDEADDEMON_H
