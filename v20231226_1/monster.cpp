#include "monster.h"
#include "monsters/catburglar.h"
#include "monsters/ironwall.h"
#include "monsters/mimicchest.h"
#include "monsters/snowman.h"
#include "monsters/twinhead.h"
#include "monsters/stall.h"
#include "monsters/puppeteer.h"

QString Game::Monster::description()
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

Game::Monster* Game::Monster::new_monster(Game::MONSTER_ID monst, Game* G)
{
    //现在G总是有monster的。这可能需要改。
    delete G->monster;G->monster=NULL;
    switch (monst)
    {
    case Game::TWIN_HEAD:
        return new Twin_Head(G);
    case Game::MIMIC_CHEST:
        return new Mimic_Chest(G);
    case Game::CAT_BURGLAR:
        return new Cat_Burglar(G);
    case Game::SNOWMAN:
        return new Snowman(G);
    case Game::IRON_WALL:
        return new Iron_Wall(G);
    case Game::STALL:
        return new Stall(G);
    case Game::PUPPETEER:
        return new Puppeteer(G);

    default:
        //qDebug()<<"UNRECOGNIZED MONSTER ID!";
        throw 0;
        return new Game::Monster(G);
    }
}

void Game::Monster::Monster_Before_Turn()//普通：回合开始
{
    G->pool->ncurrent = 1;
    Game::Piece t = G->pool->drawout();
    G->pool->current[0] = t;

    G->sync_record();
    emit G->signal_update_turn_piece();
}


void Game::Monster::Monster_Before_Combat()
{
    //发牌在[G->record.pieces[G->turn]]，操作在[G->record[turn]]，上轮分在[G->player->prev_point]
    return;
}

void Game::Monster::Monster_Combat()
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


void Game::Monster::Monster_After_Combat()
{
    return;
}


