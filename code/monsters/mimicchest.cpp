#include "mimicchest.h"
#include "game.h"
#include "cardpool.h"

Mimic_Chest::Mimic_Chest(Game* G) : Monster(G)
{
    id = GT::MIMIC_CHEST;
    name = "会说话的宝箱"; shortname = "宝箱怪";
    initialhealth = 1111;
    icon = QString("res/icon/monster/") + "2_mimic_chest.jpg";
}


QString Mimic_Chest::description()
{
    QString str = "第二期蜂巢副本BOSS：" + this->name + " 初始血量:" + QN(initialhealth) + "\n\n";
    str += "被动技能1：【贪敛】每一回合，增长[本回合卡牌右上数值(2,6,7或10)]等量的战力。\n\n";
    str += "被动技能2：【窃取】若挑战者连续至少三回合战力无变化，下次变化时获得[X*N/5]点护盾。X为挑战者变化前战力，N为无变化的回合数。(开局除外)\n\n";
    str += "主动技能：【财宝归我了！】当宝箱怪战力达到50/100/150/200时，查看后续6张卡牌。其中含1的牌作为后续若干回合的发牌；洗回其余的。\n\n";
    str += "副本环境·寻宝狂潮：卡池中含6的卡牌数量改为3张。";
    return str;
}

void Mimic_Chest::reset() {
    Monster::reset();
    nFutureTiles = 0;
    next_future = 0;
    turn_of_last_change = 0;
    ready_for_ability_1 = false;
}


void Mimic_Chest::Monster_Before_Turn()
{
    //发牌
    if (ready_for_ability_1)
    {
        trigger_ability_1();
    }
    Piece p;

    for (; next_future < nFutureTiles; next_future++)
    {
        if (iskeep(future_tiles[next_future]))break;
    }

    Piece next;
    if (next_future == nFutureTiles)
    {
        Monster::Monster_Before_Turn(); next = G->pool->current[0];
    }
    else
    {
        next = future_tiles[next_future]; G->pool->current[0] = next;
    }

    next_future++;
    if (next_future == nFutureTiles + 1)//没找到块
    {
        nFutureTiles = 0; next_future = 0;
    }

    int gain_pt = next.x267();

    G->Alert_monster("宝箱怪【贪敛】下回合增加" + QN(gain_pt) + "分");
    if (point / 50 < (point + gain_pt) / 50)
        emit G->Alert_monster("宝箱怪【财宝归我了！】即将发动：回合结束后揭示后面6张牌，将其中的非1牌洗回。");

    G->sync_record();
    emit G->signal_update_turn_piece();
}

void Mimic_Chest::trigger_ability_1()
{
    ready_for_ability_1 = false;
    for (int i = 0; i < 6; i++)
    {
        future_tiles[nFutureTiles] = G->pool->drawout(); nFutureTiles++;
    }
    emit G->Alert_monster("宝箱怪已达到" + QN(point / 50 * 50) + "分，揭示牌堆顶6张牌:");
    bool haskeep = false, hasnonkeep = false;
    QString draw_info = "", keep_info = "保留的牌有:", back_info = "洗回的牌有:";
    for (int place = -6; place <= -1; place++)
    {
        Piece p = future_tiles[nFutureTiles + place];
        draw_info += p.to_string();
        if (place != -1)draw_info += " ";
        if (iskeep(p)) { haskeep = true; keep_info = keep_info + ' ' + p.to_string(); }
        if (!iskeep(p)) { hasnonkeep = true; back_info = back_info + ' ' + p.to_string(); G->pool->pushback(p); }
    }
    emit G->Alert_monster(draw_info);
    if (haskeep)emit G->Alert_monster(keep_info);
    if (hasnonkeep)emit G->Alert_monster(back_info);
}

void Mimic_Chest::addPoint(int pt)
{
    int per50 = point / 50;
    Monster::addPoint(pt);
    if (point / 50 > per50)
        ready_for_ability_1 = true;
    return;
}

void Mimic_Chest::Monster_Before_Combat()
{
    //发牌在[G->record.pieces[G->turn]，操作在[G->record[turn]]，上轮分在[G->player->prev_point]

    if (turn_of_last_change > 0)
    {
        int turns_not_changed_ui = G->turn - turn_of_last_change;

        if (G->player->point() != G->player->prev_point)turns_not_changed_ui = 0;
        emit G->Alert_monster("未动回合数:" + QN(turns_not_changed_ui));
    }
    Piece p = G->record.pieces[G->turn][0];
    {
        int gainpt = p.x267();
        emit G->Alert_monster("宝箱怪【贪敛】触发，+" + QN(gainpt) + "分!");
        addPoint(gainpt);
    }

    if (G->player->point() != G->player->prev_point)
    {
        if (turn_of_last_change < G->turn - 3)
            if (turn_of_last_change > 0)
            {
                int turns_not_changed = G->turn - turn_of_last_change - 1;
                int armor_coef = std::min(turns_not_changed, 10);
                int armor_gain = G->player->point() * armor_coef / 5;
                emit G->Alert_monster("已有" + QN(turns_not_changed) + "回合未改变分数，宝箱怪的【窃取】触发，宝箱怪获得\n"\
                    + QN(G->player->prev_point) + "*" + QN(armor_coef) + "/5=" + QN(armor_gain) + "点护盾!");
                gainArmor(armor_gain);
            }
        turn_of_last_change = G->turn;
    }
}
