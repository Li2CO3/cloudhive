#ifndef IRON_WALL_H
#define IRON_WALL_H

#include "stdafx.h"
#include "monster.h"

class Iron_Wall : public Monster
{
public:
    Iron_Wall(Game* G);
    ~Iron_Wall() override {}
    void reset() override;
    QString description() override;
    POOLTYPE pooltype() override { return GT::POOL_IRON_WALL; } // 卡牌类型
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;


private:
    int realarmor;
    int temparmor;
};

#endif // IRON_WALL_H
