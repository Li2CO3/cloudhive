#include "stall.h"

Stall::Stall(Game* G) :Monster(G) {
    id = Game::STALL;
    name = "摆摊仙人"; shortname="saiwei";
    initialhealth = 2000;
    icon = QString("res/icon/monster/") + "6_saiwei.jpg";
}

void Stall::reset() {
    Game::Monster::reset();
}

QString Stall::description() {
    QString str = "YAMI设计："+ this->name + "\n初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能0：【热修】每回合开始时，分数增加 -5 ~ 15\n\n";
    str += "被动技能1：【保底】分数最小为0\n\n";
    str += "副本环境：无\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}


// 回合开始前
void Stall::Monster_Before_Turn() {
    //int u = (initialhealth - this->health) / 100;
    //int mi = -5 + u;
    //int ma = 10 + u;
    int k = G->random.randint(-5, 15);
    emit G->Alert_monster(name + "【热修】发动 \n/rd i " + QN(-5) + " "+ QN(15) + " \n -> "
                          + QN(k) + "\n"+ name + "获得" + QN(k) + "点分数");
    if(k<-point){k=-point; emit G->Alert_monster(name+"【保底】触发，分数不低于0!");}
    int real_increase=std::max(-point,k);

    addPoint(real_increase);

    Game::Monster::Monster_Before_Turn();
}

void Stall::Monster_Before_Combat() {
    Game::Monster::Monster_Before_Combat();
}


void Stall::Monster_Combat() {
    Game::Monster::Monster_Combat();
}

void Stall::Monster_After_Combat() {
    Game::Monster::Monster_After_Combat();
}
