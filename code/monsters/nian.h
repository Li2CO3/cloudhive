#ifndef NIAN_H
#define NIAN_H

#include "../monster.h"

class Nian_Monster : public Monster
{
public:
    Nian_Monster(Game* G);
    ~Nian_Monster(){}

    void reset() override;
    QString description() override;
    QString stat_string() override;


    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    //void Monster_After_Combat() override;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_NIAN; }

    void addChill(int chil);
    void Chill_Combat();
private:
    bool has_located[20]={false};//甩炮
    int v159[5]={0},v267[5]={0},v348[5]={0};
    int chill=0;//寒气值，负数对应暖意
    int next_nline_trigger=5;
};
#endif // NIAN_H
