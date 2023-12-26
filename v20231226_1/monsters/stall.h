#ifndef YAMI_STALL_H
#define YAMI_STALL_H

#include "../monster.h"

class Stall : public Game::Monster
{
public:
    Stall(Game* G);
    ~Stall() override {}
    void reset() override;
    QString description() override;
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
};

#endif // YAMI_STALL_H
