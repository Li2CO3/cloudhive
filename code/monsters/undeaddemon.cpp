#include "undeaddemon.h"
#include "game.h"
#include "grid.h"
#include <queue>
#include <vector>

Undead_Demon::Undead_Demon(Game* G) :Monster(G) {
    id = GT::UNDEAD_DEMON;
    name = "亡骨魔";
    shortname="亡骨魔";
    initialhealth = 600;
    icon = QString("res/icon/monster/") + "10_undeaddemon.jpg";
    rescout = 0;
}

void Undead_Demon::reset() {
    Monster::reset();
    rescout = 0;
}

QString Undead_Demon::description() {
    QString str = "圣墓上的倒吊人设计："+ this->name + "\n初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能0：【不死圣泉】限三次，血量小于0时，回满血并且战力减少一半复活\n\n";
    str += "被动技能1：【抉择】每回合结束后，战力增加3+10*X点(X为不死圣泉发动次数)\n\n";
    str += "被动技能2：【亡骨之毒】不死圣泉发动时，消除挑战者盘内点数最高的卡牌两张\n\n";
    str += "主动技能3：【巫婆】每8回合发动一次，回合开始时，将挑战者盘内随机一张9的卡牌中数字9变成数字1\n\n";
    str += "副本环境·钥匙：卡池中没有0含1的卡牌，含9的卡牌改为4张\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}


// 回合开始前
void Undead_Demon::Monster_Before_Turn() {
    emit G->Alert_monster(name + "【抉择】发动. 战力增加了" + QN(3 + 10 * rescout));
    this->addPoint(3 + 10 * rescout);

    if(G->turn % 8 == 0) {
        std::vector<int> ninevec;
        for (int i = 1; i <= 19; i++) {
            Piece t = G->player->grid.pieces[i];
            if (t.x159() == 9) {
                ninevec.push_back(i);
            }
        }
        if (ninevec.size() > 0) {
            int index = G->random.randint(1, ninevec.size()) - 1;
            Piece t = G->player->grid.pieces[ninevec[index]];
            emit G->Alert_monster(name + "【巫婆】发动. 将位置" + QN(ninevec[index]) + "的卡牌"
                                  + t.to_string() + "变成" + G->player->grid.pieces[ninevec[index]].to_string());
            G->player->Locate_piece(Piece(t.x348(), 1, t.x267()), ninevec[index]);
        } else {
           emit G->Alert_monster(name + "【巫婆】发动失败");
        }
    }

    Monster::Monster_Before_Turn();
}

void Undead_Demon::Monster_After_Combat() {
    Monster::Monster_After_Combat();
    if(this->health < 0 && rescout < 3) {
        emit G->Alert_monster(name + "【不死圣泉】发动，血量增加700，战力减少" + QN(this->point / 2) + "点");
        this->health = this->initialhealth;
        this->addPoint(this->point / -2);
        ++rescout;

        // 同时触发的亡骨之毒
        clear_piece();
    }
}

void Undead_Demon::clear_piece() {
    // 一种牌最多4张，暴力点随机
    std::priority_queue<pair<int, int>> queue;
    for (int i = 1; i <= 19; i++) {
        Piece t = G->player->grid.pieces[i];
        queue.push(make_pair(t.sum(), i));
    }
    int nums[7];// 虽然5不可能越界但我还是写了7//没有用vector
    int len = 0;
    // 先选两张
    nums[len++] = queue.top().second;
    int sumone = queue.top().first;
    queue.pop();
    nums[len++] = queue.top().second;
    int sumtwo = queue.top().first;
    queue.pop();
    while (queue.top().first == sumtwo && !queue.empty()) {
        // 如果有多张只可能等于第二张
        nums[len++] = queue.top().second;
        queue.pop();
    }
    int one = 0;
    int two = 1;
    if (len > 2) {
        // 大于2时随机选择
        if(sumone == sumtwo) {
            // 全部都是同一种卡牌
            do {
                one = G->random.randint(1, len) - 1;
                two = G->random.randint(1, len) - 1;
            } while(one == two);
        }
        else {
            // 第一张卡牌是最大的，只随机后面的
            two = G->random.randint(2, len) - 1;
        }
    }
    G->player->Locate_piece(Piece(0, 0, 0), nums[one]);
    G->player->Locate_piece(Piece(0, 0, 0), nums[two]);
    emit G->Alert_monster(name + "【亡骨之毒】发动. 消除了位置" + QN(nums[one]) + "和位置" + QN(nums[two]) + "的卡牌");
    G->Locate_Piece();

}

