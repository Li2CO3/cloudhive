﻿#include "nian.h"
#include "game.h"

#ifdef NewYear2024NianEasy
Nian_Easy::Nian_Easy(Game* G) :Nian_Monster(G) {
	id = GT::NIAN_EASY;
	name = "赖子年兽";
	shortname = "赖子年";
	initialhealth = 20240;
	//chill=0;
	//icon = QString("res/icon/monster/") + "9plus_nian_easy.jpg";
}

QString Nian_Easy::description() {
	QString str = "2024快乐副本：" + this->name + " 初始血量:" + QN(initialhealth) + "\n";
	str += "年兽规则基础上新增一些规则, 其它规则见[年兽]页\n\n";
	str += "副本环境·龙年快乐:\n";
	str += "①增加18个赖子(共计20个纯赖子和27个单线赖子)。\n";
	str += "②大地红威力增加为原先的3倍。\n";
	str += "③用含赖子棋子触发鞭炮时效果翻倍。\n";
	return str;
}

#endif

Nian_Monster::Nian_Monster(Game* G) :Monster(G) {
	id = GT::NIAN_MONSTER;
	name = "年兽"; shortname = "年兽";
	initialhealth = 2024;
	chill = 0;
	icon = QString("res/icon/monster/") + "9_nian.jpg";
}

void Nian_Monster::reset() {
	Monster::reset();
	this->chill = 0;
	for (int place = 1; place <= 19; place++) this->has_located[place] = false;
	for (int lineid = 0; lineid < 5; lineid++) { v159[lineid] = 0; v267[lineid] = 0; v348[lineid] = 0; }
	next_nline_trigger = 5;
}

QString Nian_Monster::description() {
	QString str = "2024副本之巢挑战赛：" + this->name + " 初始血量:" + QN(initialhealth) + "\n";
	str += "被动技能①·年纪：每回合增长12点战力。\n";
	str += "被动技能②·蛰伏：每5回合才会主动攻击一次。\n";
	str += "主动技能①·岁末扫除：回合开始，蓄力10点[寒气]。攻击时会消耗所有[寒气]值并造成等量的伤害\n";
	//str += "主动技能②·大年三十：入腊，气候严寒，同类深匿，无可为餐，乃离山入市，觅人饱腹，遇难幸免，迨除夕闻爆竹声，方惊去。";
	str += "主动技能②·熬年守岁：年兽只会战斗" + QN(this->max_turn()) + "回合\n";
	str += "棋盘环境·鞭炮：燃放威力为X的鞭炮可使年兽战力降低X点并施放X点暖意。[暖意]与[寒气]优先互相抵消，挑战者攻击时可对年兽造成等量真实伤害。\n";
	str += "①·甩炮：棋盘上每个格子上有甩炮，首次放置卡牌时触发。威力：10-(编号与10的差值)。\n";
	str += "②·大地红：首次连线时，燃放大地红以庆祝。威力：(2*连线长度)。\n";
	str += "③·窜天猴：挑战者盘面连线数首次达到5/10/15时，燃放窜天猴以点燃气氛。威力：12/24/48。\n";

	str += "棋子环境·龙年大吉：卡池中新增单边赖子卡牌各一张。\n";

	return str;
}

QString Nian_Monster::stat_string()
{
	return Monster::stat_string() + '[' + QN(chill) + ']';
}

void Nian_Monster::addChill(int chil)
{
	chill += chil;

	emit G->Alert_monster("当前寒气值为" + QN(chill));
	//emit G->signal_monster_change_stats(point, health, armor);
}

// 回合开始前
void Nian_Monster::Monster_Before_Turn() {
	emit G->Alert_monster(name + "【年纪】战力增加12，寒气增加10!");
	addPoint(12);
	addChill(10);

	Monster::Monster_Before_Turn();
}

void Nian_Monster::Monster_Before_Combat() {
	//发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player()->prev_point]
	Piece p = G->record.pieces[G->turn][0];//本回合放的块
	int mult = get_mult(p);

	int place = G->record[G->turn].toInt();

	if (place > 0) if (this->has_located[place] == false)
	{
		has_located[place] = true;
		//触发甩炮
		int val = mult * ( /*2**/(10 - std::abs(10 - place)));
		emit G->Alert_monster("【甩炮】触发，削弱" + name + QN(val) + "点战力，并增加等量暖意!");
		addPoint(-val);
		addChill(-val);
	}

	{
		int temp159[5], temp267[5], temp348[5];
		//int pt=
		G->player()->grid.point(GT::Scoring_Normal, temp159, temp267, temp348);
		int newlinescount = 0;
		for (int lineid = 0; lineid < 5; lineid++)
		{
			if (temp159[lineid] > v159[lineid]) { newlinescount += GT::linelength[lineid]; v159[lineid] = 10; }
			if (temp267[lineid] > v267[lineid]) { newlinescount += GT::linelength[lineid]; v267[lineid] = 10; }
			if (temp348[lineid] > v348[lineid]) { newlinescount += GT::linelength[lineid]; v348[lineid] = 10; }
			// v159[lineid]=temp159[lineid];
			// v267[lineid]=temp267[lineid];
			// v348[lineid]=temp348[lineid];
		}

		if (newlinescount > 0)
		{
			//大地红
			int val = mult * (get_earth_red_power() * newlinescount);
			emit G->Alert_monster("【大地红】触发，削弱" + name + QN(val) + "点战力，并增加等量暖意!");
			addPoint(-val);
			addChill(-val);
		}
	}

	{
		int nl = G->player()->grid.point(GT::COUNT_LINES);
		if (nl >= this->next_nline_trigger)
		{
			int dmg;
			switch (next_nline_trigger)
			{
			case 5: dmg = 12; break;
			case 10:dmg = 24; break;
			case 15:dmg = 48; break;
			default:throw 0;
			}
			next_nline_trigger += 5;
			//窜天猴
			int val = mult * dmg;

			emit G->Alert_monster("【窜天猴】触发，削弱" + name + QN(val) + "点战力，并增加等量暖意!");
			addPoint(-val);
			addChill(-val);
		}
	}

	Monster::Monster_Before_Combat();
}

void Nian_Monster::Chill_Combat()
{
	if (chill > 0)
	{
		deal_damage(chill);
		emit G->Alert_monster(name + "消耗了" + QN(chill) + "点寒气并造成!" + QN(chill) + "点伤害!");
		addChill(-chill);
	}
	else if (chill == 0)
	{
		emit G->Alert_monster("寒气和暖意抵消了!");
	}
	else if (chill < 0)
	{
		emit G->Alert_monster("暖意对" + name + "造成了" + QN(-chill) + "点伤害!");
		take_damage(-chill);
	}
}

void Nian_Monster::Monster_Combat() {
	if (G->turn % 5 == 0)
	{
		Monster::Monster_Combat();
		this->Chill_Combat();
	}
	else
	{
		if (G->player()->point() > point)
		{
			Monster::Monster_Combat();
			if (chill < 0)
				this->Chill_Combat();
		}
		else
		{
			emit G->Alert_monster(name + "【蛰伏】中...");
		}
	}
	return;
}

//void Nian_Monster::Monster_After_Combat() {
//    Monster::Monster_After_Combat();
//}