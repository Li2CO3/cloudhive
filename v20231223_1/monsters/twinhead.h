#ifndef TWIN_HEAD_H
#define TWIN_HEAD_H

#include "monster.h"

class Twin_Head :public Game::Monster//双头龙
{
public:
    Twin_Head(Game* G);
    ~Twin_Head() override {}
    POOLTYPE pooltype() override { return POOLTYPE::POOL_TWIN_HEAD; }
    QString description() override;

    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
};


#endif // TWIN_HEAD_H
