#ifndef GAME_H
#define GAME_H

#define NUMCOMB_MAX_PLAYER 10
#define NUMCOMB_MAX_TURN 20
#define PVE_MAX_TURN 35

#include <QApplication>
#include <time.h>
#include <QThread>
#include <QFontDatabase>

#include "stdafx.h"
#include "utils/random.hpp"
#include "utils/gametools.h"
#include "piece.h"
#include "grid.h"

using namespace std;

#define LAIZI "any"
class MainWindow;//自定义窗口类

class CardPool;

class Monster;

class Player;

class Game :public QObject//QThread//游戏进行在另一个线程
{
	Q_OBJECT

public:

    enum GAMESTAGE { NOT_STARTED = 0, WAIT_TURN = -1, FINISHED = 2, WAIT_CHOOSE_TURN = -3, BUSY = 100, WAIT_SNOWMAN_DISCARD = -4004, };

    enum GAME_RESULT { PLAYER_DEAD = 0, TURNS_ENOUGH = 1 };



	class Record
	{
	public:
		int npieces[101];
		Piece pieces[101][9];
		Piece covered[101];
		QString cache[101];
		QString operation[101];
		QString& operator[](int t) { return operation[t]; }
	};

	void Game_End()
	{
        status = FINISHED; emit signal_game_end(); //exit();
	}

    Game();
    ~Game() ;
	//todo:log

	void Clear() {}

    int turn = 0;
    Random *random;
    GT::SCORING_RULE scoring_rule;
    GT::GAMETYPE gametype;
    GAMESTAGE status;
	MainWindow* MW;
    virtual CardPool* pool(int id=0){Q_UNUSED(id) return NULL;}///原本的pool player monster是现在的pool() player() monster()。
    virtual Player* player(int id=0) {Q_UNUSED(id) return NULL;}
    virtual Monster* monster(int id=0){Q_UNUSED(id) return NULL;}
    virtual void setPoolLink(CardPool *pol,unsigned int id=0){Q_UNUSED(pol) Q_UNUSED(id)}
    virtual void setPlayerLink(Player *ply, unsigned int id=0){Q_UNUSED(ply) Q_UNUSED(id)}
    virtual void setMonster(Monster *monst, unsigned int id=0){Q_UNUSED(monst) Q_UNUSED(id)}
//    virtual Player* player(int id=0) =NULL;
//    virtual Monster *monster(int id=0) =NULL;

	Record record;
    virtual void sync_record(){}

    virtual bool Check_Operation(int ply, QString cache, QString op){ Q_UNUSED(ply) throw 0;}


    virtual void load_challenge(GT::MONSTER_ID monst){Q_UNUSED(monst) }
	//int monster_extra_info[10];//特殊信息

	//    totaldamage=0; monsterpoint=0; monsterhealth=monsterinitialhealth; monsterarmor=0;
	//    for(int i=0;i<10;i++)monster_extra_info[i]=0;
	//    player_health=150; monsterdead=false;playerdead=false;
	//    grid.Clear();
	//{init_boss_health=0;boss_health=0;boss_armor=0;}

	//void set_challenge(int id);
	//void set_monster(int id);
	//void showinfo();
    //int Prompt_select_monster();
    virtual void make_basic_connections(){}
    virtual void Start(QString seed){}//todo:参数
    virtual void Before_Turn(){}
    virtual void Locate_Piece(){}
    virtual void Before_Combat(){}
    virtual void Combat(){}
    virtual void After_Combat(){}
    virtual void setrandom(QString seed);
    //void run() override { }
    void remake(){remake(gametype);}
    void remake(GT::GAMETYPE type);
public slots:
    virtual void recv_operation(QString op){}
signals:
    void signal_new_operation(QString);
    void signal_before_turn();
    void signal_turn();
    void signal_before_combat();
    void signal_combat();
    void signal_after_combat();
    void signal_update_turn_piece();
    void signal_player_change_stats(QString,int=0);//需要输入stat_string()TODO: stat_string这个词可能要换
    void signal_monster_change_stats(QString);//需要输入stat_string()
    void signal_wait_for_operation();
    void signal_game_end();
    void Alert_monster(QString);
};

class GameMaker
{
public:
    static Game * newGame(GT::GAMETYPE type);
};
#endif // GAME_H


///Game现在可以近似认为是抽象类。
///程序启动时创建的是Game。在点击开始游戏时会重新创建一个PveGame。
///现在remake()处会进行Game信息的复制，但对player和monster可能并不完整。在新模式推出时(?)需验证
