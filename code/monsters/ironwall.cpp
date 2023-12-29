#include "ironwall.h"
#include "game.h"
#include "cardpool.h"

Iron_Wall::Iron_Wall(Game* G) :Monster(G) {
    id = GT::IRON_WALL;
    name = "铁壁主宰"; shortname = "铁壁";
    initialhealth = 288;
    icon = QString("res/icon/monster/") + "5_ironwall.jpg";
    realarmor = 0;
    temparmor = 0;
}

void Iron_Wall::reset() {
    Monster::reset();
    realarmor = 0;
    temparmor = 0;
}

QString Iron_Wall::description() {
    QString str = "Anidlebrain特制BOSS："+ this->name + "\n初始血量:" + QN(initialhealth) + "\n\n";
    str += "主动技能0：【虚空吞噬】第六回合后，开始战斗之前，吞噬(销毁)一张卡牌，本回合临时增加卡牌点数的护盾。\n\n";
    str += "被动技能1：【永恒守护】每个回合开始时，若护盾值为0，增加挑战者战力的护盾。\n\n";
    str += "被动技能2：【反击之盾】战斗时，战力等于当前的护盾。\n\n";
    str += "副本环境·破晓战场：卡池中卡牌点数点数和是质数的卡牌数量改为5张。\n";
    str += "(317 397 412 416 452 496 812 856 892 896)\n";
    return str;
}

// 回合开始前
void Iron_Wall::Monster_Before_Turn() {
    if (armor <= 0) {
        armor = G->player->point();
        emit G->Alert_monster(name+"【永恒守护】发动，护盾变为"+QN(armor));
    }
    Monster::Monster_Before_Turn();
}

void Iron_Wall::Monster_Before_Combat() {
    if (G->turn < 6) {
        return ;
    }
    Piece piece = G->pool->drawout();
    temparmor = piece.sum();
    emit G->Alert_monster(name+"【虚空吞噬】发动，吞噬了" + piece.to_string() + "获得+" + QN((temparmor)) + "点临时护盾！");
    realarmor = armor;
    armor += temparmor;
    Monster::Monster_Before_Combat();
}


void Iron_Wall::Monster_Combat() {
    this->point = armor;
    emit G->Alert_monster(name+"【反击之盾】发动，当前战力为"+QN(point));
    Monster::Monster_Combat();
    this->point = 0;
}

void Iron_Wall::Monster_After_Combat() {
    if (realarmor < armor) {
        emit G->Alert_monster(name+"临时护盾减少"+QN(armor-realarmor)+"点");
        armor = realarmor;
        emit G->signal_monster_change_stats(stat_string());
    }
    Monster::Monster_After_Combat();
}
