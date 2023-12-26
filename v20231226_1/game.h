#ifndef GAME_H
#define GAME_H

#define MAX_TURN 35//不确定改了会不会出情况，但是工程内搜索35应该不会漏太多

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

class MainWindow;//自定义窗口类

class CardPool;

class Monster;

class Game :public QObject//QThread//游戏进行在另一个线程
{
	Q_OBJECT

public:

    enum GAMESTAGE { NOT_STARTED = 0, WAIT_TURN = -1, FINISHED = 2, WAIT_CHOOSE_TURN = -3, BUSY = 100, WAIT_SNOWMAN_DISCARD = -4004 };
    enum GAMEMODE { MODE_NULL=0,MODE_PVE = 1 };
    enum GAME_RESULT { PLAYER_DEAD = 0, TURNS_ENOUGH = 1 };

	class Player//玩家
	{
	public:
		Game* G;
		QString name;//名字
		QString icon;//头像的连接。TODO 现在格式和Monster的不一致。因为做选怪页面先把怪的给改了
		Grid grid;
		int health;
		bool dead;//如果死了就变true
		int totaldamage;//玩家打出的伤害
		int prev_point;//上轮分数
		int point() { return grid.point(G->scoring_rule); }//里面注释掉的版本是适配开放巢的，但不方便设置1加分，就先注释掉了
		void Locate_piece(Piece p, int place);//放块，更改Player里的东西，向Window发出更改显示的signal
		Player(Game* g) { G = g; Clear(); }
		void reset() { totaldamage = 0; prev_point = 0; dead = false; health = 150; grid = Grid(); }//开局状态。如果需要改可能还得跳转到Monster::去
		void Clear() { name = "挑战者"; icon = "lgtbot.jpg"; reset(); }//除了构造函数，没有别处调用

		void load_name();//看res/game/user.txt设置名字
		void set_name(QString name);
		void set_icon(QString icon);

        void changePoint(int pt) { emit G->signal_player_change_stats(pt, health); }//没有point变量。只是发一个信号
		void take_damage(int dmg)//默认的受到伤害
		{
			health -= dmg;
            emit G->Alert_monster(name + "血量减少" + QString::number(dmg));
            emit G->signal_player_change_stats(point(), health);
            if (health <= 0) { dead = true; emit G->Alert_monster(name + "已被击杀..."); G->Game_End(); }
		}
	};

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
    Random random;
    GT::SCORING_RULE scoring_rule;
    GAMEMODE mode;
    GAMESTAGE status;
	MainWindow* MW;
    CardPool* pool;
    Player* player;
    Monster* monster;
//    virtual Player* player(int id=0) =NULL;
//    virtual Monster *monster(int id=0) =NULL;

	Record record;
    void sync_record();

	bool Check_Operation(int ply, QString cache, QString op);

    void Start(GAMEMODE mode);//todo:参数

    void load_challenge(GT::MONSTER_ID monst);
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
	void Before_Turn();
	void Locate_Piece();
	void Before_Combat();
	void Combat();
	void After_Combat();
    void RUN(GAMEMODE mode) {  Start(mode); }//并没有开线程
    //void run() override { }
	void remake();
public slots:
	void recv_operation(QString op);
signals:
	void signal_new_operation(QString);
	void signal_before_turn();
	void signal_turn();
	void signal_before_combat();
	void signal_combat();
	void signal_after_combat();
	void signal_update_turn_piece();
	void signal_player_change_stats(int, int);//point,health
	void signal_monster_change_stats(int, int, int);//point,health,armor
	void signal_wait_for_operation();
	void signal_game_end();
	void Alert_monster(QString);
};

#endif // GAME_H
