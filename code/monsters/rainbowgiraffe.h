#ifndef RAINBOWGIRAFFE_H
#define RAINBOWGIRAFFE_H
#include "stdafx.h"
#include <set>
#include "../monster.h"

class RainbowGiraffe : public Monster
{
public:
    RainbowGiraffe(Game* G);
    ~RainbowGiraffe() override {}
    void reset() override;
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_RAINBOW_GIRAFFE; }
private:
    std::set<int> uniqueSet;
};

#endif // RAINBOWGIRAFFE_H
