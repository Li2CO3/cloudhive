#include "snowman.h"
#include <QLabel>
#include "game.h"
#include "cardpool.h"
#include "mainwindow.h"

Snowman::Snowman(Game* G) :Monster(G) {
    id = GT::SNOWMAN;
    name = "雪人"; shortname = "雪人";
    initialhealth = 2888;
    icon = QString("res/icon/monster/") + "4_snowman.jpg";
    }

void Snowman::reset() {
    Monster::reset();
    frozen = false; illusion_point = 0;
    damage_snowman_taken = 0, damage_player_taken = 0;
    is_in_illusion = false; cards_discarded = 0;
}

QString Snowman::description()
{
    QString str = "第四期蜂巢副本BOSS：雪人 初始血量:" + QN(initialhealth) + "\n";

    str += "被动技能1: 【积雪】每一回合，获得[当前卡牌数字总和]的护盾。护盾持续一回合。\n";

    str += "被动技能2: 【堆雪】正常回合里，雪人每轮增长2点战力。挑战者战力如有变化则改为增长5点。\n";

    str += "主动技能1: 【打雪仗】雪人召开领域，挑战者仿佛回到童年，与雪人一起打雪仗。";
    str += "第20回合结束后，双方进入类似于平行世界的领域里打雪仗。雪仗持续6回合，开始时雪人战力为100。\n";
    str += "玩家丢弃一张盘面上的牌，对雪人造成[牌面数字总和]的真实伤害，雪人获得等量战力，然后获得该轮的发牌并放置。双方交战然后进行下一回合，交战暂时不结算伤害。";
    str += "如此6回合后，结算交战时的伤害，并回到第20轮，开始正常的第21轮。\n";
    str += "主动技能2: 【结冰】正常回合里，当雪人战力达到100时，将永久进入冰封状态，自身获得150点格挡与25%反伤。\n";
    str += "副本环境·雪人、树枝与冬：每列1额外获得15分。";
    return str;
}

void Snowman::Monster_Before_Turn()//混乱屎山代码
{
    if (G->turn == 21) { Go_to_illusion(); }
    if (is_in_illusion && G->turn == 27 && cards_discarded == 0) { Go_out_of_illusion(); }
    if (G->status == Game::FINISHED)return;

    if (!is_in_illusion)
    {
        if (G->turn >= 27 || G->turn < 21)
        {
            Monster::Monster_Before_Turn();
        }
        else {
            Piece p = cache_pieces[G->turn - 20];
            G->pool->ncurrent = 1;
            G->pool->current[0] = p;
            G->sync_record();
            emit G->signal_update_turn_piece();
        }
    }
    if (is_in_illusion)
    {
        if (cards_discarded != 0)G->turn--;

        if (cards_discarded == 1) {
            Monster::Monster_Before_Turn();
            cards_discarded = 0;
            cache_pieces[G->turn - 20] = G->pool->current[0]; }
        else {
            G->pool->ncurrent = 1;
            G->pool->current[0] = Piece(0,0,0);
            emit G->signal_update_turn_piece();
        }
    }
}
void Snowman::setStatus()
{
    if (G->pool->current[0].isEmptyPiece())
    {
        G->status = Game::WAIT_SNOWMAN_DISCARD; return;
    }
    else
    {
        Monster::setStatus();
    }
}

void Snowman::Monster_Before_Combat()
{
    if (G->turn == 20 || (is_in_illusion && (G->turn < 26 || G->pool->current[0].isEmptyPiece())))
    {
        emit G->Alert_monster("!打雪仗中...!");
    }
    if (is_in_illusion)
    {
        //if(this->cards_discarded==0)cache_o[G->turn-20]="";
        if (G->pool->current[0].isEmptyPiece())
        {
            cache_o[G->turn - 20] += "-";
        }
        else
        {
            cache_o[G->turn - 20] += "+";
        }
        if (G->record[G->turn].toInt() < 10)cache_o[G->turn - 20] += " ";
        cache_o[G->turn - 20] += QN(G->record[G->turn].toInt());
    }
    if (is_in_illusion && G->pool->current[0].isEmptyPiece())
    {
        cards_discarded++;
        Piece p = G->record.covered[G->turn];
        int damage = p.x159() + p.x348() + p.x267();
        emit G->Alert_monster(G->player->name + "丢弃" + p.to_string() + ",造成" + QN(damage) + "点伤害!");
        take_real_damage(damage);
        if (is_in_illusion) { addPoint(damage); }
        emit G->Alert_monster("雪人获得" + QN(damage) + "积分!");

        //                //弃掉的是covered
    }

    if (!is_in_illusion)//正常回合
    {
        int gain_pt = 2;
        if (G->player->point() != G->player->prev_point)gain_pt = 5;
        {
            emit G->Alert_monster("雪人【堆雪】发动，+" + QN(gain_pt) + "分！");
            addPoint(gain_pt);
        }

        if (G->turn == 20) { turn_20_grid = G->player->grid; turn_20_point = point; }
    }

    Piece p = G->pool->current[0];
    if (!p.isEmptyPiece())//不是空牌
    {
        int piece_point = p.x159() + p.x267() + p.x348();
        emit G->Alert_monster("雪人【积雪】发动，获得+" + QN(piece_point) + "临时护盾！");
        prev_armor = armor; gainArmor(piece_point);
        //emit )
    }

    // Game::Monster::Monster_Before_Combat();
}
void Snowman::take_damage(int damage)
{
    if (is_in_illusion) {
        if (armor >= damage)
        {
            emit G->Alert_monster("雪人临时护盾减少" + QN(damage) + "，抵消了伤害!"); armor -= damage;
            emit G->signal_monster_change_stats(stat_string());
        }
        else
        {
            emit G->Alert_monster("雪人护盾被被击穿，" + QN(damage - armor) + "点伤害延迟结算!");
            this->damage_snowman_taken += (damage - armor);
            armor = 0;
        }
        return;
    }
    else
        Monster::take_damage(damage);
    return;
}
void Snowman::deal_damage(int dmg)
{
    if (is_in_illusion) { this->damage_player_taken += dmg; emit G->Alert_monster("该伤害在雪仗结束后结算"); }
    else G->player->take_damage(dmg);
}
void Snowman::take_real_damage(int damage)
{
    Monster::take_damage(damage);
}
void Snowman::addPoint(int pt)
{
    Monster::addPoint(pt);
    if (point >= 100) if (!is_in_illusion) { frozen = true; }
}

void Snowman::Monster_Combat()
{
    if (G->turn < 3)return;
    if (is_in_illusion)if (G->pool->current[0].isEmptyPiece())
    {
        return;
    }
    int mypt = G->player->point();
    int monsterpt = G->monster->point;
    int difference = mypt - monsterpt;
    int damage = difference;
    if (mypt > monsterpt)
    {
        emit G->Alert_monster(G->player->name + " vs " + name + "(-" + QN(difference) + ")");
        if (frozen)
        {
            if (damage <= 150)
            {
            emit G->Alert_monster("雪人【结冰】获得150格挡，伤害抵消!"); return;
            }
            else
            {
                emit G->Alert_monster("雪人【结冰】格挡，伤害-150并具有反伤!"); damage -= 150;
            }
        }
        take_damage(damage);
        if (frozen)
        {
            int revenge = damage / 4;
            if (revenge > 0)
            {
                emit G->Alert_monster("雪人【结冰】反伤25%，造成" + QN(revenge) + "伤害!");
                deal_damage(revenge);
            }
        }
    }
    if (mypt == monsterpt)
    {
        emit G->Alert_monster(G->player->name + " vs " + name + "(-0)");
    }
    if (mypt < monsterpt)
    {
        damage = -difference;//变成正的
        emit G->Alert_monster(G->player->name + "(-" + QN(damage) + ")" + " vs " + name);
        deal_damage(damage);
    }
}

void Snowman::Monster_After_Combat()
{
    if (is_in_illusion)if (G->pool->current[0].isEmptyPiece())
    {
        return;
    }
    Monster::Monster_After_Combat();
    if (armor > prev_armor)
    {
        int diff = armor - prev_armor; armor = prev_armor;
        emit G->Alert_monster("雪人失去临时护盾" + QN(diff) + "点");
        emit G->signal_monster_change_stats(stat_string());
    }

    if (is_in_illusion)
    {
        emit G->Alert_monster(G->player->name + "已受伤害: " + QN(damage_player_taken));
        emit G->Alert_monster("雪人已受伤害: " + QN(damage_snowman_taken));
    }
}


void Snowman::Make_Summary(QDialog* dialog)

{
    Monster::Make_Summary(dialog);
    QLabel* ql;
    if (G->turn >= 21)
    {
        NEW_LABEL_ALIGN(ql, 375, 300, 200, 200, "", 16, Left, dialog);
        QFont f = QFont("consolas"); f.setPixelSize(16);
        ql->setFont(f);

        for (int i = 1; i <= 6; i++)
        {
            ql->setText(ql->text());
            QString g = ql->text();
            g = g + cache_o[i] + "/";
            ql->setText(g);
            if (i == 3)ql->setText(ql->text() + "\n");
        }
    }
}

void Snowman::Go_out_of_illusion() {
    is_in_illusion = false;
    emit G->Alert_monster("雪仗结束，结算伤害！");
    take_damage(damage_snowman_taken);
    G->player->take_damage(damage_player_taken);
    G->turn = 21;
    G->player->grid = turn_20_grid;
    G->player->prev_point = G->player->point();
    point = turn_20_point;
    G->MW->page->update();
}
