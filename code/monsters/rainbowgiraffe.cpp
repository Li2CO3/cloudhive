﻿#include "rainbowgiraffe.h"
#include "game.h"
#include "cardpool.h"

RainbowGiraffe::RainbowGiraffe(Game* G) :Monster(G) {
    id = GT::RAINBOW_GIRAFFE;
    name = "彩色长颈鹿"; shortname="长颈鹿";
    initialhealth = 1777;
    icon = QString("res/icon/monster/") + "8_rainbowgiraffe.jpg";
    uniqueSet.insert(10);
}

void RainbowGiraffe::reset() {
    Monster::reset();
    uniqueSet.clear();
    uniqueSet.insert(10);
}

QString RainbowGiraffe::description() {
    QString str = "圣墓上的倒吊人设计："+ this->name + "\n初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能0：【彩虹的颜色】每回合结束前，分数增加 2 ~ 7\n\n";
    str += "被动技能1：【三原色】开局时的三张卡的各个数字不重合\n\n";
    str += "主动技能2: 【棱镜】每7回合发动一次，盘里每有一张赖子或带赖子的卡，就对玩家造成7点伤害";
    str += "副本环境·彩虹：卡池内新增一条边带有赖子的卡牌每种一张\n";
    str += "由于图片的限制暂为3X2、4X6、8X7、39X、45X、81X、X16、X57、X92、XXX\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}


// 回合开始前
// 发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player->prev_point]

void RainbowGiraffe::Monster_Before_Turn() {
    if (G->turn <= 3) {
        // 【三原色】
        G->pool->ncurrent = 1;
        Piece t = G->pool->drawout();
        while (true) {
            if (uniqueSet.find(t.x159()) == uniqueSet.end() &&
                    uniqueSet.find(t.x267()) == uniqueSet.end() &&
                    uniqueSet.find(t.x348()) == uniqueSet.end()) {
                   uniqueSet.insert(t.x159());
                   uniqueSet.insert(t.x267());
                   uniqueSet.insert(t.x348());
                   break;
            }
            G->pool->pushback(t);
            t = G->pool->drawout();
        }
        emit G->Alert_monster(name + "【三原色】发动\n本回合的牌为" + t.to_string());
        G->pool->current[0] = t;
        G->sync_record();
        emit G->signal_update_turn_piece();
    }
    else {
        Monster::Monster_Before_Turn();
    }

}

void RainbowGiraffe::Monster_Before_Combat() {
    Monster::Monster_Before_Combat();
}


void RainbowGiraffe::Monster_Combat() {
    Monster::Monster_Combat();
    if (G->turn % 7 == 0) {
        int cnt = 0;
        for (int i = 1; i < 19; i++) {
           Piece t = G->player->grid.pieces[i];
           if (t.x159() == 10 || t.x267() == 10 || t.x348() == 10) {
               cnt++;
           }
        }
        if (cnt > 0) {
            emit G->Alert_monster(name + "【棱镜】发动\n对你造成了" + QN(cnt * 7) + "点伤害");
            G->player->take_damage(cnt * 7);
        }
        else {
            emit G->Alert_monster(name + "【棱镜】发动\n发现你没有癞子！");
        }

    }
}

void RainbowGiraffe::Monster_After_Combat() {
    Monster::Monster_After_Combat();
    {//【彩虹的颜色】
        int increase_point = G->random.randint(2, 7);
        this->addPoint(increase_point);
        emit G->Alert_monster(name + "【彩虹的颜色】发动\nroll出了" + QN(increase_point) + "点");
    }
}