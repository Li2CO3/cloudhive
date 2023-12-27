#ifndef RAINBOWGIRAFFE_H
#define RAINBOWGIRAFFE_H
#include "stdafx.h"
#include "../monster.h"

class rainbowGiraffe : public Monster
{
public:
    rainbowGiraffe(Game* G);
    ~rainbowGiraffe() override {}
    void reset() override;
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
};

#endif // RAINBOWGIRAFFE_H
