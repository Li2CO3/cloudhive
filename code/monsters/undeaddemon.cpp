#include "undeaddemon.h"
#include "game.h"
#include "grid.h"
#include <queue>
#include <vector>

Undead_Demon::Undead_Demon(Game* G) :Monster(G) {
    id = GT::UNDEAD_DEMON;
    name = "亡骨魔";
    shortname = "亡骨魔";
    initialhealth = 500;
    icon = QString("res/icon/monster/") + "10_undeaddemon.jpg";
    rescount = 0;
    dead = true;//这是亡灵，dead=true很正常对吧
    undead_random = new SimpleRandom();
}

void Undead_Demon::reset() {
    Monster::reset();
    rescount = 0;
}
QString Undead_Demon::stat_string()
{
    return Monster::stat_string() + '[' + '#' + QN(this->rescount + 1) + ']';
}
QString Undead_Demon::description() {
    QString str = "圣墓上的倒吊人设计：" + this->name + " 初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能0：【不死圣泉】限三次，血量小于0时，回满血并且战力减少一半复活\n\n";
    str += "被动技能1：【抉择】每回合结束后，战力增加3+10*X点(X为不死圣泉发动次数)\n\n";
    str += "被动技能2：【亡骨之毒】不死圣泉发动时，消除挑战者盘内点数最高的卡牌两张(赖子按30计算)\n\n";
    str += "主动技能3：【巫婆】每8回合发动一次，回合开始时，将挑战者盘内随机一张9的卡牌中数字9变成数字1\n\n";
    str += "副本环境·钥匙：卡池中没有0含1的卡牌，含9的卡牌改为4张\n";
    str += "注意: 尚未确认此副本平衡性，请谨慎参考初始血量\n";
    return str;
}

// 回合开始前
void Undead_Demon::Monster_Before_Turn() {
    if (G->turn == 1) { undead_random->setseed(G->random->getseed()); }

    emit G->Alert_monster(name + "【抉择】即将发动. 增加" + QN(3 + 10 * rescount) + "战力!");
    //this->addPoint(3 + 10 * rescount);

    if (G->turn % 8 == 7) {
        emit G->Alert_monster(name + "【巫婆】即将发动，随机将一个9替换为1!");
    }
    if (G->turn % 8 == 0) {
        std::vector<int> ninevec;
        for (int i = 1; i <= 19; i++) {
            Piece t = G->player()->grid.pieces[i];
            if (t.x159() == 9) {
                ninevec.push_back(i);
            }
        }
        if (ninevec.size() > 0) {
            int index = undead_random->randint(1, ninevec.size()) - 1;
            Piece t = G->player()->grid.pieces[ninevec[index]];
            G->player()->Locate_piece(Piece(t.x348(), 1, t.x267()), ninevec[index]);
            emit G->Alert_monster(name + "【巫婆】发动. 将位置" + QN(ninevec[index]) + "的卡牌"
                + t.to_string() + "变成" + G->player()->grid.pieces[ninevec[index]].to_string());
        }
        else {
            emit G->Alert_monster(name + "【巫婆】发动失败");
        }
    }

    Monster::Monster_Before_Turn();
}

void Undead_Demon::Monster_Before_Combat() {
    Monster::Monster_Before_Combat();
    emit G->Alert_monster(name + "【抉择】发动. 战力增加" + QN(3 + 10 * rescount));
    this->addPoint(3 + 10 * rescount);
}
void Undead_Demon::Monster_After_Combat() {
    Monster::Monster_After_Combat();
    if (this->health <= 0 && rescount < 3) {
        emit G->Alert_monster(name + "【不死圣泉】发动，血量增加" + QN(initialhealth) + "，战力减少为" + QN(this->point / 2) + "点");
        emit G->Alert_monster("此后" + name + "每回合将增长" + QN(3 + 10 * rescount + 10) + "!");
        this->health += this->initialhealth;
        this->addPoint(-(point - (point / 2)));
        ++rescount;

        if (rescount == 3) { dead = false; }//复活次数用尽，再打死可以恭喜了
        // 同时触发的亡骨之毒
        clear_piece();
    }
}
int Undead_Demon::get_pos_of_largest()
{
    //由于是重生时才看，不会出现 全盘全空的情况
    int sum = -1;
    int target = -1;
    int ntie = 0;
    Grid& gr = G->player()->grid;

    for (int place = 1; place <= 19; place++)
    {
        if (gr[place]->sum() >= 0)
        {
            if (gr[place]->sum() == sum)
            {
                if (undead_random->randint(1, ntie + 1) == ntie + 1) {
                    target = place;
                }
                ntie += 1;
            }
            else if (gr[place]->sum() > sum)
            {
                sum = gr[place]->sum(); target = place; ntie = 1;
            }
        }
    }
    if (target == -1)throw 0;
    return target;
}

void Undead_Demon::clear_piece() {
    int firsttarget, secondtarget;
    firsttarget = get_pos_of_largest();
    G->player()->Locate_piece(Piece(), firsttarget);
    secondtarget = get_pos_of_largest();
    G->player()->Locate_piece(Piece(), secondtarget);
    emit G->Alert_monster(name + "【亡骨之毒】发动. 消除了位置" + QN(firsttarget) + "和位置" + QN(secondtarget) + "的卡牌");
}
