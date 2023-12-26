#include "rainbowgiraffe.h"
#include "game.h"
#include "cardpool.h"

rainbowGiraffe::rainbowGiraffe(Game* G) :Monster(G) {
    id = GT::STALL;
    name = "彩色长颈鹿"; shortname="长颈鹿";
    initialhealth = 700;
    icon = QString("res/icon/monster/") + "6_saiwei.jpg";
}

void rainbowGiraffe::reset() {
    Monster::reset();
}

QString rainbowGiraffe::description() {
    QString str = "圣墓上的倒吊人设计："+ this->name + "\n初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能0：【彩虹的颜色】每回合结束前，分数增加 2 ~ 7\n\n";
    str += "被动技能1：【三原色】开局时的三张卡的各个数字不重合\n\n";
    str += "主动技能2: 【棱镜】每7回合发动一次，盘里每有一张赖子或带赖子的卡，就对玩家造成7点伤害";
    str += "副本环境·彩虹：卡池内新增一条边带有赖子的卡牌每种一张\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}


// 回合开始前
// 发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player->prev_point]

void rainbowGiraffe::Monster_Before_Turn() {
    Monster::Monster_Before_Turn();
}

void rainbowGiraffe::Monster_Before_Combat() {
    Monster::Monster_Before_Combat();
}


void rainbowGiraffe::Monster_Combat() {
    Monster::Monster_Combat();
}

void rainbowGiraffe::Monster_After_Combat() {
    Monster::Monster_After_Combat();
    int increase_point = G->random.randint(2, 7);
    this->addPoint(increase_point);
    emit G->Alert_monster(name + "【彩虹的颜色】发动roll出了" + QN(increase_point) + "点");
}
