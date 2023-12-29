#include "rainbowgiraffe.h"
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
    str += "被动技能0：【彩虹的颜色】每回合结束前，抽取一种彩虹的颜色，然后战力增加1~7\n\n";
    str += "被动技能1：【三原色】挑战者开局的三张卡牌的各个数字均不重合\n\n";
    str += "主动技能2: 【棱镜】每7回合发动一次，盘里每有一张赖子或带赖子的卡牌，就对挑战者造成7点伤害\n\n";
    str += "副本环境·彩虹：卡池内新增一条边带有赖子的卡牌每种一张\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}

int RainbowGiraffe::Count_LAIZI() {
    int cnt = 0;
    for (int i = 1; i <= 19; i++) {
       Piece t = G->player->grid.pieces[i];
       if (t.x159() == 10 || t.x267() == 10 || t.x348() == 10) {
           cnt++;
       }
    }
    return cnt;
}

// 回合开始前
// 发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player->prev_point]
void RainbowGiraffe::Monster_Before_Turn() {
    int cnt = Count_LAIZI();
    emit G->Alert_monster("当前赖子数:" + QN(cnt));
    emit G->Alert_monster(name+"【彩虹的颜色】下回合增加1~7分");
    if (G->turn <= 3) {
        // 【三原色】
        G->pool->ncurrent = 1;
        Piece t = G->pool->drawout();//抽卡
        while (true) {
            if (uniqueSet.find(t.x159()) == uniqueSet.end() &&
                    uniqueSet.find(t.x267()) == uniqueSet.end() &&
                    uniqueSet.find(t.x348()) == uniqueSet.end()) {//如果没有重复数字，记录数字
                   uniqueSet.insert(t.x159());
                   uniqueSet.insert(t.x267());
                   uniqueSet.insert(t.x348());
                   break;
            }
            G->pool->pushback(t);//如果有重复数字了，放回去
            t = G->pool->drawout();//重新抽
        }
        emit G->Alert_monster(name + "【三原色】发动\n本回合的牌为" + t.to_string());
        G->pool->current[0] = t;
        G->sync_record();
        emit G->signal_update_turn_piece();
    }
    else {
        Monster::Monster_Before_Turn();
    }
    if(G->turn %7 ==0)
    {
        emit G->Alert_monster(name+"【棱镜】即将发动，根据盘面上赖子数量造成伤害!");
    }

}

const QString RainbowGiraffe::rainbowColour[] = {
    "红色",
    "橙色",
    "黄色",
    "绿色",
    "蓝色",
    "靛色",
    "紫色",
};

void RainbowGiraffe::Monster_Before_Combat() {
    Monster::Monster_Before_Combat();
    {//【彩虹的颜色】
        int increase_point = G->random.randint(1, 7);
        this->addPoint(increase_point);
        emit G->Alert_monster(name + "【彩虹的颜色】发动\n选中了"
                              + rainbowColour[increase_point - 1] + ",分数增加" + QN(increase_point) + "点");
    }

    if (G->turn % 7 == 0) {
        int cnt = Count_LAIZI();
        if (cnt > 0) {
            emit G->Alert_monster(name + "【棱镜】发动，造成了" + QN(cnt * 7) + "点伤害");
            G->player->take_damage(cnt * 7);
        }
        else {
            emit G->Alert_monster(name + "【棱镜】发动，但并没有发现赖子！");
        }

    }
}



//void RainbowGiraffe::Monster_Combat() {
//    Monster::Monster_Combat();
//}

//void RainbowGiraffe::Monster_After_Combat() {
//    Monster::Monster_After_Combat();
//}
