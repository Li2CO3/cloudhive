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
    virtual int max_turn() override {return 30;}
    void addChill(int chil);
    void Chill_Combat();
protected:
    virtual int get_mult(Piece p) {return 1;};
    virtual int get_earth_red_power() {return 2;};
    bool has_located[20]={false};//甩炮
    int v159[5]={0},v267[5]={0},v348[5]={0};
    int chill=0;//寒气值，负数对应暖意
    int next_nline_trigger=5;
};

#ifdef NewYear2024NianEasy
class Nian_Easy :public Nian_Monster
{
public:
    Nian_Easy(Game *G);
    ~Nian_Easy(){}
    QString description() override ;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_NIAN_EASY; }
protected:
    int get_mult(Piece p) override {return 1 + (p.x159() == 10 || p.x267() == 10 || p.x348() == 10);};
    int get_earth_red_power() override{return 6;};
};
#endif//2024快乐年

#endif // NIAN_H
