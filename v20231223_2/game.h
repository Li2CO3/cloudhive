#ifndef GAME_H
#define GAME_H

//#define emit X){//qDebug()<<#X; emit X;}
//#define emit X) emit X//造成了一堆warning
#define QN(X) QString::number(X)

#define MAX_XUANXIU 9//给一些容器设置大小。目前只用到2.

#define MAX_TURN 35//不确定改了会不会出情况，但是工程内搜索35应该不会漏太多

#include <QApplication>
#include <QString>
#include <iostream>
#include <time.h>
#include <QThread>
#include <QFontDatabase>
#define MAX_POOL 200//卡池上限 改了没事 就是会影响class Pool大小
using namespace std;

//...|#26|...|#32|...|#36|...
//#20|...|#30|...|#34|...|#40
//...|#27|...|--8|...|#37|...
//#21|...|--4|...|-13|...|#41
//...|--1|...|--9|...|-17|...
//#22|...|--5|...|-14|...|#42
//...|--2|...|-10|...|-18|...
//#23|...|--6|...|-15|...|#43
//...|--3|...|-11|...|-19|...
//#24|...|--7|...|-16|...|#44
//...|#28|...|-12|...|#38|...
//#25|...|#31|...|#35|...|#45
//...|#29|...|#33|...|#39|...


#define GT gametools
namespace gametools
{

double const GRID_X[46] = { 5,1,1,1, 2,2,2,2, 3,3,3,3,3, 4,4,4,4, 5,5,5,
                           0,0,0,0,0,0, 1,1,1,1, 2,2,3,3,4,4, 5,5,5,5, 6,6,6,6,6,6 };
int const GRID_Y[46] = { 1,5,7,9, 4,6,8,10, 3,5,7,9,11, 4,6,8,10, 5,7,9,
                        2,4,6,8,10,12, 1,3,11,13, 2,12, 1,13, 2,12, 1,3,11,13, 2,4,6,8,10,12 };

int const place_ids[15][7] = {
    { 0, 0, 0, 0, 0, 0, 0},
    { 0,26, 0,32, 0,36, 0},
    {20, 0,30, 0,34, 0,40},
    { 0,27, 0, 8, 0,37, 0},
    {21, 0, 4, 0,13, 0,41},
    { 0, 1, 0, 9, 0,17, 0},
    {22, 0, 5, 0,14, 0,42},
    { 0, 2, 0,10, 0,18, 0},
    {23, 0, 6, 0,15, 0,43},
    { 0, 3, 0,11, 0,19, 0},
    {24, 0, 7, 0,16, 0,44},
    { 0,28, 0,12, 0,38, 0},
    {25, 0,31, 0,35, 0,45},
    { 0,29, 0,33, 0,39, 0},
    { 0, 0, 0, 0, 0, 0, 0}
};//给开放世界巢做准备→ →

enum PIECES {EMPTY_PIECE=0, LAIZI=1000};

enum SCORING_RULE { Scoring_Normal = 0, One_Lines_Plus_12 = 112, One_Lines_Plus_15 = 115, COUNT_LINES=-2147483647 };

enum PoolType {POOL_Normal=0,POOL_TWIN_HEAD=-4, POOL_MIMIC_CHEST=66, POOL_CAT_BURGLAR=GT::LAIZI, POOL_IRON_WALL=235711, POOL_PUPPETEER=123};
//setpool

enum GAME_RESULT { PLAYER_DEAD = 0, TURNS_ENOUGH = 1 };//UNUSED

};

class MainWindow;//自定义窗口类


class Game :public QThread//游戏进行在另一个线程
{
	Q_OBJECT

public:
	class Random//随机数 摇骰子都在这里
	{
	private:
		int seed;//mt19937的事情，下次一定，下次一定
		qint64 now;
	public:
		void setseed(int newseed) { seed = newseed; now = seed; }
		int getseed() { return seed; }
		int generate() {//随机生成一个0~32767的数字
			now = 214013 * now;
			now &= 0x7FFFFFFF;
			now = (now + 2531011);

			return (now >> 16) & 0x7FFF;
		}
		int randint(int min, int max)//含上下界
		{
			int range = max - min + 1;
			int a = generate();
			while (a / range == 0x7FFF / range) { a = generate(); }
			return a % range + min;
		}
		Random() { seed = time(NULL); now = seed; }//默认取时间
	};
	Random random;

	class Piece
	{
	public:
		int id;//万能牌是1000.空牌是0. 以后可能要区分全断的000和空牌。例如312是312,497是497
		int x348() const { if (id == 1000)return 10; return id / 100; }//万能牌是10,否则返回3/1/2线值
		int x159() const { if (id == 1000)return 10; return (id / 10) % 10; }
		int x267() const { if (id == 1000)return 10; return id % 10; }
        bool isNormalPiece()
        {
            if(id==GT::LAIZI)return true;
            if(id/100==3|| id/100==4 || id/100==8)
                if((id/10)%10==1 || (id/10)%10==5 || (id/10)%10==9)
                    if(id%10==2 || id%10==6 || id%10==7)
                        return true;
            return false;
        }
		int const ValueofLine(int x) const //返回x线值
		{
			if (x == 3 || x == 4 || x == 8)return x348();
			if (x == 2 || x == 6 || x == 7)return x267();
			if (x == 1 || x == 5 || x == 9)return x159();
			throw 0;
		}
		QString to_string()
		{
			if (id == 1000)return "any";
			if (id == 0)return "---";
			QString temp = "xxx";
			temp[0] = QChar('0' + x348());
			temp[1] = QChar('0' + x159());
			temp[2] = QChar('0' + x267());
			return temp;
		}
        int sum() {
            return x348() + x267() + x159();
        }
		Piece() { id = 0; }//默认是空牌
		Piece(int info) { id = info; }
		operator int() { return id; }
		//operator QString(){return to_string();}//整理注释的时候加的，所以也先带上注释
	};

	class Pool
	{
	public://自己乱编，无脑public，危
		Game* G;
		int npiece;//卡池张数
		Piece pieces[MAX_POOL];//卡池
		Piece current[MAX_XUANXIU];//当前回合的块
		int ncurrent;//当前回合块数
        int nPool(GT::PoolType type, Piece p);
        void setpool(GT::PoolType type);//根据种类设置卡池//TODO:把type改成enum
        Pool(Game* g) {
            G = g;
            npiece=0;
            setpool(GT::POOL_Normal);
        }
        Piece drawout() {
            int k = G->random.randint(0, npiece - 1);
            Piece p = pieces[k];
            pieces[k] = pieces[npiece - 1];
            npiece--;
            return p;
        }//卡池里随机移除一张作为返回值，把最后一块移到空位
        void back(Piece p) {
            pieces[npiece] = p;
            npiece++;
            return;
        }//把一张卡放回来(放到最后)
	};

	class Grid//盘面
	{
	public:
		Piece pieces[20];//1~19是1~19号位。0号是凑数的
        //int prev_point;//上回合分数，boss们曾经会看，遗留产物我直接注释掉
        int point() { return point(GT::Scoring_Normal); };//计分
        int point(GT::SCORING_RULE rule);//计分，带规则
        int nlines(){return point(GT::COUNT_LINES);}//计算线数 用开放巢的方式做的
		Piece* operator[](int t) { return &pieces[t]; }//返回盘面几号位。这个感觉可以去掉
		Grid() { Clear(); }
		void Clear() { for (int i = 0; i < 20; i++)pieces[i] = 0; }//清除上回合分数和盘面

		void Make_image(QPixmap& pixmap);//画成图。用LGTBot的图片资料
	};

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

    enum MONSTER_ID {
        DEFAULT_MONSTER = 0, TWIN_HEAD = 1, SHUANGTOULONG = 1,
        MIMIC_CHEST = 2, BAOXIANGGUAI = 2, HUISHUOHUADEBAOXIANG = 2,
        NAMI = 3, CAT_BURGLAR = 3, XIAOZEIMAO = 3, HAIZEIWANG = 3,
        SNOWMAN = 4, XUEGUAI = 4,
        IRON_WALL = 5,
        STALL = 6,
        PUPPETEER = 7,
    };
	class Monster
    {
    public:

		Game* G;
		QString name;
        QString shortname;
		QString icon;
        MONSTER_ID id;
		int initialhealth;//在构造的时候就设置上了
		int point;
		int health;
		int armor;
		bool dead;
        Monster(Game* g) { G = g; G->monster=this; name = ""; shortname=""; id = DEFAULT_MONSTER; initialhealth = 0; dead = false; }
        virtual ~Monster() {}
		static Monster* new_monster(MONSTER_ID monst, Game* G);//把Game的Monster替换成一个新的，根据monst创建

		virtual QString description();//描述
		virtual void reset() { health = initialhealth; armor = 0; point = 0; }
		virtual void setStatus() {//发完牌马上可以操作的时候，看下怪的状态决定是时进行哪类操作：为了适配雪人，雪人撕牌不能撕空的和0
            if (G->pool->ncurrent == 1) {
                G->status = WAIT_TURN;
            }
            else if (G->pool->ncurrent > 1) {
                for(int i = 0; i< 10; ++i);
                G->status = WAIT_CHOOSE_TURN;
            }
			else//pool->ncurrent==0?
            {
                for(int i = 0; i< 10; ++i);
                throw 0;//现在没有跳过回合，所以直接throw
            }
		}
        virtual GT::PoolType pooltype() { return GT::POOL_Normal; }//怪对应的牌堆类型
        virtual GT::SCORING_RULE scoringrule() { return GT::Scoring_Normal; }//计分规则。
		virtual void Monster_Before_Turn();//包含发牌
		virtual void Monster_Before_Combat();//发牌在[G->record.pieces[G->turn]]，备注在[G->record.cache[turn]]操作在[G->record[turn]]，上轮分在[G->player->prev_point]
		virtual void Monster_Combat();//打架效果，包含掉血
		virtual void Monster_After_Combat();//打架后效果
        virtual void addPoint(int pt) { point += pt; emit G->signal_monster_change_stats(point, health, armor); }//这个signal是给窗口看的//右边这是什么意思我已经看不懂了→//todo:加一个place
        void gainArmor(int arm) { armor += arm; emit G->signal_monster_change_stats(point, health, armor); }
		virtual void deal_damage(int dmg) { G->player->take_damage(dmg); }
		virtual void take_damage(int dmg) {
			G->player->totaldamage += dmg;
			if (armor == 0)
			{
				health -= dmg;
                emit G->Alert_monster(name + "血量减少" + QString::number(dmg));
                emit G->signal_monster_change_stats(point, health, armor);
			}
			else if (armor > dmg)
			{
				armor -= dmg;
                emit G->Alert_monster(name + "护盾减少" + QString::number(dmg));
                emit G->signal_monster_change_stats(point, health, armor);
			}
			else if (armor == dmg)
			{
				armor -= dmg;
                emit G->Alert_monster(name + "护盾减少" + QString::number(dmg) + ",刚好被击穿");
                emit G->signal_monster_change_stats(point, health, armor);
			}
			else if (armor < dmg)
			{
				int excess = dmg - armor;
				armor = 0; health -= excess;
                emit G->Alert_monster(name + "护盾被击穿，且血量减少" + QString::number(excess));
                emit G->signal_monster_change_stats(point, health, armor);
			}

			if (health <= 0)
			{
                if (!dead) { emit G->Alert_monster("恭喜你击败" + name + "!"); }
				dead = true;
			}
		}

        virtual void Make_Summary(QDialog* dialog);
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
	enum GAME_RESULT { PLAYER_DEAD = 0, TURNS_ENOUGH = 1 };
	void Game_End()
	{
		status = FINISHED; emit signal_game_end(); exit();
	}

	int turn = 0;

	Game() {
		connect(this, SIGNAL(signal_new_operation(QString)), this, SLOT(recv_operation(QString)));

		Clear();
		connect(this, &Game::signal_before_turn, this, [=]() {Before_Turn(); });
		connect(this, &Game::signal_turn, this, [=]() {Locate_Piece(); });
		connect(this, &Game::signal_before_combat, this, [=]() {Before_Combat(); });
		connect(this, &Game::signal_combat, this, [=]() {Combat(); });
		connect(this, &Game::signal_after_combat, this, [=]() {After_Combat(); });
		status = NOT_STARTED;
		mode = MODE_PVE;
		pool = new Pool(this);
		player = new Player(this);
		monster = new Monster(this);
	}
	~Game() {
        //qDebug()<<"REMAKED";
		if (pool)delete pool;  if (player)delete player; if (monster)delete monster;//Debug()<<"DELETE GAME";
	}

    GT::SCORING_RULE scoring_rule;

	//todo:log

	void Clear() {}
	MainWindow* MW;
	Pool* pool;
	Player* player;
	Monster* monster;
	Record record;
	void sync_record() {//TODO
		record.npieces[turn] = pool->ncurrent;
		for (int i = 0; i < pool->ncurrent; i++)record.pieces[turn][i] = pool->current[i];
	}

	enum GAMESTAGE { NOT_STARTED = 0, WAIT_TURN = -1, FINISHED = 2, BUSY = -1, WAIT_CHOOSE_TURN = -3, WAIT_SNOWMAN_DISCARD = -4004 };
	enum GAMEMODE { MODE_PVE = 1 };
	GAMEMODE mode;
	GAMESTAGE status;
	bool Check_Operation(int ply, QString cache, QString op);

	void Start();//todo:参数

    void load_challenge(MONSTER_ID monst)
	{
		//pool=new Pool(this);
		//player=new Player(this);
		//现在pool和player没什么特殊的。如果需要可以参照monster

        Monster::new_monster(monst, this);
		scoring_rule = monster->scoringrule();
		pool->setpool(monster->pooltype());
		player->health = 150;
		return;
	}
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
	void RUN() { run(); }
	void run() override { Start(); }
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
