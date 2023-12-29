#include "monster.h"
#include "monsters/catburglar.h"
#include "monsters/ironwall.h"
#include "monsters/mimicchest.h"
#include "monsters/snowman.h"
#include "monsters/twinhead.h"
#include "monsters/stall.h"
#include "monsters/puppeteer.h"
#include "monsters/rainbowgiraffe.h"
#include "game.h"
#include "cardpool.h"

QString Monster::description()
{
    QString str = "PvE怪物挑战中的怪物。具有较高血量和一些能力。\n";
    //str+="Monster是所有怪物的基类，保存有所有怪物的共同属性。";
    str += "当迎战具体的怪物时，将可以显示相应信息。\n";
    str += "--------\n";
    str += "每个怪物有其对应的卡池。如无特殊能力影响，玩家每回合从中抽取一块，填入其中一块。后面的规则以后再补。";
    str += "这里的文字不会显示出来。不信我在这里加个throw";
    //throw 0;
    return str;
}
QString Monster::stat_string()
{
    QString str= QN(point)+'/'+QN(health);
    if(armor>0) str=str+'<'+QN(armor)+'>';
    return str;
}

Monster::Monster(Game* g) {
    G = g;
    G->monster=this;
    name = "";
    shortname="";
    id = GT::DEFAULT_MONSTER;
    initialhealth = 0;
    dead = false;
}
Monster::~Monster() {}

Monster* Monster::new_monster(GT::MONSTER_ID monst, Game* G)
{
    //现在G总是有monster的。这可能需要改。
    using namespace GT;
    delete G->monster;G->monster=NULL;
    switch (monst)
    {
    case TWIN_HEAD:
        return new Twin_Head(G);
    case MIMIC_CHEST:
        return new Mimic_Chest(G);
    case CAT_BURGLAR:
        return new Cat_Burglar(G);
    case SNOWMAN:
        return new Snowman(G);
    case IRON_WALL:
        return new Iron_Wall(G);
    case STALL:
        return new Stall(G);
    case PUPPETEER:
        return new Puppeteer(G);
    case RAINBOW_GIRAFFE:
        return new RainbowGiraffe(G);
    default:
        //qDebug()<<"UNRECOGNIZED MONSTER ID!";
        throw 0;
        return new Monster(G);
    }
}

void Monster::Monster_Before_Turn()//普通：回合开始
{
    G->pool->ncurrent = 1;
    Piece t = G->pool->drawout();
    G->pool->current[0] = t;

    G->sync_record();
    emit G->signal_update_turn_piece();
}


void Monster::Monster_Before_Combat()
{
    //发牌在[G->record.pieces[G->turn]]，操作在[G->record[turn]]，上轮分在[G->player->prev_point]
    return;
}

void Monster::Monster_Combat()
{
    if (G->turn < 3)return;
    int mypt = G->player->point();
    int monsterpt = G->monster->point;
    int difference = mypt - monsterpt;
    int damage = difference;
    if (mypt > monsterpt)
    {
        emit G->Alert_monster(G->player->name + " vs " + name + "(-" + QN(difference) + ")");
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


void Monster::Monster_After_Combat()
{
    return;
}

void Monster::setStatus() {//发完牌马上可以操作的时候，看下怪的状态决定是时进行哪类操作：为了适配雪人，雪人撕牌不能撕空的和0
    if (G->pool->ncurrent == 1) {
        G->status = Game::WAIT_TURN;
    }
    else if (G->pool->ncurrent > 1) {
        for(int i = 0; i< 10; ++i);
        G->status = Game::WAIT_CHOOSE_TURN;
    }
                else//pool->ncurrent==0?
    {
        //for(int i = 0; i< 10; ++i);
        throw 0;//现在没有跳过回合，所以直接throw
    }
}

void Monster::addPoint(int pt) {
    point += pt; emit G->signal_monster_change_stats(stat_string());//point, health, armor);
}

void Monster::gainArmor(int arm) {
    armor += arm;
    emit G->signal_monster_change_stats(stat_string());
}

void Monster::take_damage(int dmg) {
    G->player->totaldamage += dmg;
    if (armor == 0)
    {
        health -= dmg;
        emit G->Alert_monster(name + "血量减少" + QString::number(dmg));
        emit G->signal_monster_change_stats(stat_string());
    }
    else if (armor > dmg)
    {
        armor -= dmg;
        emit G->Alert_monster(name + "护盾减少" + QString::number(dmg));
        emit G->signal_monster_change_stats(stat_string());
    }
    else if (armor == dmg)
    {
        armor -= dmg;
        emit G->Alert_monster(name + "护盾减少" + QString::number(dmg) + ",刚好被击穿");
        emit G->signal_monster_change_stats(stat_string());
    }
    else if (armor < dmg)
    {
        int excess = dmg - armor;
        armor = 0; health -= excess;
        emit G->Alert_monster(name + "护盾被击穿，且血量减少" + QString::number(excess));
        emit G->signal_monster_change_stats(stat_string());
    }

    if (health <= 0)
    {
        if (!dead) {
            emit G->Alert_monster("恭喜你击败" + name + "!");
        }
        dead = true;
    }
}

void Monster::deal_damage(int dmg) {
    G->player->take_damage(dmg);
}
