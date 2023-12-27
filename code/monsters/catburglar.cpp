#include "catburglar.h"
#include <QLabel>
#include "ui.h"
#include "game.h"
#include "cardpool.h"

Cat_Burglar::Cat_Burglar(Game* G) : Monster(G) {
    id = GT::CAT_BURGLAR;
    name = "小贼猫";
    shortname = "小贼猫";
    initialhealth = 1000;
    icon = QString("res/icon/monster/") + "3_cat_burglar.jpg";
}
void Cat_Burglar::reset() {
    Monster::reset();
    bomb1 = 0;
    bomb2 = 0;
}//bomb是欺骗技能的位置


QString Cat_Burglar::description()
{
    QString str = "第三期蜂巢副本boss：小贼猫 初始血量:" + QN(initialhealth) + "\n";
    str += "被动技能1：【丰满】第20回合时，获得100点战力。\n";
    str += "被动技能2：【聪慧】从第一回合起，每回合获得3点战力，若当回合挑战者分数有变化，则改为获得9点战力。\n";
    str += "主动技能1：【欺骗】第11-20和21-30回合中各有一个炸弹，挑战者在该回合未弃牌则受到其放置卡牌的总点数点伤害，最高不超过15点。\n";
    str += "主动技能2：【天候棒】小贼猫依据战况操控场上天气，设定附加规则：\n";
    str += "·当挑战者连线数为3-6时，触发雷电天候，对挑战者每回合造成4点伤害；\n";
    str += "·当挑战者连线数为7-10时，触发狂风天候，挑战者每回合的砖块改为从两张中选择其一；\n";
    str += "·当挑战者连线数大于10时，触发浓雾天候，当回合小贼猫具有10%减伤。\n";//视线不清的挑战者当回合对小贼猫造成的伤害降低10%

    str += "副本环境·大海贼时代：卡池中赖子数量改为5。";
    return str;
}

void Cat_Burglar::Monster_Before_Turn()
{
    Grid gr = G->player->grid;
    int nl = gr.nlines();

	if (nl >= 7 && nl <= 10)
	{
		G->pool->current[0] = G->pool->drawout();
		G->pool->current[1] = G->pool->drawout();
		G->pool->ncurrent = 2;
        emit G->Alert_monster("狂风天候:本轮改为二选一!");
	}
	else
	{
		G->pool->current[0] = G->pool->drawout();
		G->pool->ncurrent = 1;
	}

    emit G->Alert_monster("小贼猫【聪慧】下回合增加3~9分");
	if (G->turn == 20)
        emit G->Alert_monster("小贼猫【丰满】即将发动：回合结束后增加100分！");
	if (G->turn == 1)
	{
		bomb1 = G->random.randint(11, 20); bomb2 = G->random.randint(21, 30);
	}

	if (G->turn >= 11) if (bomb1 >= G->turn)
        emit G->Alert_monster("小贼猫可能在这轮埋了炸弹!");
	if (G->turn >= 21) if (bomb2 >= G->turn)
        emit G->Alert_monster("小贼猫可能在这轮埋了炸弹!");

	G->sync_record();
    emit G->signal_update_turn_piece();
}

void Cat_Burglar::Monster_Before_Combat()
{
    emit G->Alert_monster("当前连线数:" + QN(G->player->grid.nlines()));

	int ptgain = ((G->player->prev_point != G->player->point()) ? 9 : 3);
    emit G->Alert_monster("小贼猫【聪慧】触发，获得" + QN(ptgain) + "分!");
	addPoint(ptgain);

	if (G->turn == 20) {
        emit G->Alert_monster("小贼猫【丰满】触发，获得" + QN(100) + "分!");
		addPoint(100);
	}

    if (G->player->grid.nlines() >= 3 && G->player->grid.nlines() <= 6)
	{
        emit G->Alert_monster("小贼猫【雷电天候】触发，对" + G->player->name + "造成4点伤害!");
		deal_damage(4);
	}

	int choice = G->record.cache[G->turn].toInt();
	int place = G->record.operation[G->turn].toInt();
	if (bomb1 == G->turn || bomb2 == G->turn)
	{
		if (place != 0)
		{
            Piece target = G->record.pieces[G->turn][choice];
			int damage = std::min(15, target.x159() + target.x267() + target.x348());
            emit G->Alert_monster("小贼猫【欺骗】触发，对" + G->player->name + "造成" + QN(damage) + "点伤害!");
			deal_damage(damage);
		}
		else
		{
            emit G->Alert_monster("小贼猫【欺骗】埋下的炸弹打空了！");
		}
	}
}
void Cat_Burglar::Monster_Combat()
{
	if (G->turn < 3)return;
	int mypt = G->player->point();
	int monsterpt = G->monster->point;
	int difference = mypt - monsterpt;
	int damage = difference;
	if (mypt > monsterpt)
	{
        emit G->Alert_monster(G->player->name + " vs " + name + "(-" + QN(difference) + ")");
        if (G->player->grid.nlines() > 10)
		{
            emit G->Alert_monster("小贼猫【浓雾天候】触发，伤害降低10%!");
			damage -= int(damage * 0.1);
		}
		take_damage(damage);
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

void Cat_Burglar::Make_Summary(QDialog* dialog)
{
    Monster::Make_Summary(dialog);

    for (int i = 1; i <= MAX_TURN && i <= G->turn; i++)
    {
        if (G->record.npieces[i] == 2)
        {
            Piece p = G->record.pieces[i][1 - G->record.cache[i].toInt()];
            QLabel* l;
            //piece->setGeometry(80+((i-1)%10*45),370+(i-1)/10*45,32,32);
            NEW_LABEL_IMAGE(l, 80 + ((i - 1) % 10 * 45) + 20, 370 + (i - 1) / 10 * 45 + 20, 16, 16, p.image(), dialog);
        }
    }
}

