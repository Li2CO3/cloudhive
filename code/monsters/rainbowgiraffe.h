#ifndef RAINBOWGIRAFFE_H
#define RAINBOWGIRAFFE_H
#include "stdafx.h"
#include <set>
#include "../monster.h"

class Rainbow_Giraffe : public Monster
{
public:
    Rainbow_Giraffe(Game* G);
    ~Rainbow_Giraffe() override {}
    void reset() override;
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    //void Monster_Combat() override;
    //void Monster_After_Combat() override;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_RAINBOW_GIRAFFE; }
private:
    int Count_LAIZI();
    std::set<int> uniqueSet;
    const static QString rainbowColour[];
};

#endif // RAINBOWGIRAFFE_H
