#ifndef PLANEBOSS_H
#define PLANEBOSS_H
#include "qpixmap.h"
#include "utils/random.hpp"

class PlaneBattleGame;
class PlaneBoss{
public:
    PlaneBoss(PlaneBattleGame *pg );//{PG=pg;random=PG->random;}
    PlaneBattleGame *PG;
    Random *random;
    virtual int nPlane(){return 3;}
    virtual int nRDCenter(){return 0;}

    virtual int bonusvalue(int a, int b, int c, int d) =0;
    void put_planes();
    virtual ~PlaneBoss(){}

    int strike_min=3;
    int strike_max=6;
    void normal_attack();
    virtual void special_ability()=0;
    virtual bool affect_player_strike(int x, int y){return false;}

    virtual QString name(){return "";};
    virtual QString icon(){return "";};
};

class Boss_Fire:public PlaneBoss{
public:
    Boss_Fire(PlaneBattleGame *pg):PlaneBoss(pg){}
    int nPlane() override{return 6;}

    void special_ability() override;

    void AirForce_Command();

    QString name()override {return "无限火力";}
    QString icon()override {return "res/icon/planeboss/boss_fire.png";}

    int bonusvalue(int a, int b, int c, int d) override;
};

class Boss_Nuke:public PlaneBoss{
public:
    Boss_Nuke(PlaneBattleGame *pg):PlaneBoss(pg){}


    bool nuked=false;
    int nPlane_now = 5;
    int nPlane() override{return nPlane_now;}
    int nRDCenter() override{return 1;}

    bool bRDhit();
    int pNuke();
    void AirForce_Reinforce();
    void Graphite_Bomb();

    void special_ability() override;
    bool affect_player_strike(int x, int y) override;
    QString name()override {return "核平铀好";}
    QString icon()override {return "res/icon/planeboss/boss_nuke.png";}

    int bonusvalue(int a, int b, int c, int d) override;
};

#endif // PLANEBOSS_H
