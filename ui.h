#ifndef UI_H
#define UI_H
#include "mainwindow.h"
#include "qapplication.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QImage>
#include <QDialog>
#include <QPainter>
#include "game.h"
#include "monsters/monster.h"
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

class MainWindow;

#define setG(g1,g2,g3,g4) setGeometry((g1),(g2),(g3),(g4))
#define setP(g) setPixelSize((g))
#define MC() MW->centralWidget()

#define NEW_BUTTON_MW(name,g1,g2,g3,g4,text,fontsize) NEW_BUTTON(name,rel(g1),rel(g2),rel(g3),rel(g4),text,rel(fontsize),MC())

#define NEW_BUTTON(name,g1,g2,g3,g4,text,fontsize,parent)\
{name=new QPushButton(parent);\
     name->setG(g1,g2,g3,g4);\
     name->setText(text);\
    QFont f;\
    /*f.setFamily("OPPOSans R");*/\
    f.setP(fontsize);\
    name->setFont(f);\
     name->show();}

#define NEW_LABEL_MW(name,g1,g2,g3,g4,text,fontsize)  NEW_LABEL(name,rel(g1),rel(g2),rel(g3),rel(g4),text,rel(fontsize),MC())

#define NEW_LABEL(name,g1,g2,g3,g4,text,fontsize,parent)\
{name=new QLabel(parent);\
        name->setG(g1,g2,g3,g4);\
        name->setText(text);\
    name->setWordWrap(true);\
    QFont f;\
/*f.setFamily("OPPOSans R");*/\
    f.setP(fontsize);\
    name->setFont(f);\
        name->show();}

#define NEW_LABEL_ALIGN_MW(name,g1,g2,g3,g4,text,fontsize,align) NEW_LABEL_ALIGN(name,rel(g1),rel(g2),rel(g3),rel(g4),text,rel(fontsize),align,MC())

#define NEW_LABEL_ALIGN(name,g1,g2,g3,g4,text,fontsize,align,parent)\
{NEW_LABEL(name,g1,g2,g3,g4,text,fontsize,parent); name->setAlignment(Qt::Align##align);}

#define NEW_LABEL_IMAGE_MW(name,g1,g2,g3,g4,link) NEW_LABEL_IMAGE(name,rel(g1),rel(g2),rel(g3),rel(g4),link,MC())

#define NEW_LABEL_IMAGE(name,g1,g2,g3,g4,link,parent){\
name=new QLabel(parent);\
        name->setG(g1,g2,g3,g4);\
name->setPixmap(QPixmap(link).SCALED((g3),(g4)));\
        name->show();}

#define NEW_BUTTON_IMAGE_MW(name,g1,g2,g3,g4,link) NEW_BUTTON_IMAGE(name,rel(g1),rel(g2),rel(g3),rel(g4),link,MC())

#define NEW_BUTTON_IMAGE(name,g1,g2,g3,g4,link,parent) {\
name=new QPushButton(parent);\
    name->setG(g1,g2,g3,g4);\
name->setIcon(QIcon(QPixmap(link).SCALED((g3),(g4))));\
name->setIconSize(QSize((g3),(g4)));\
    name->show();}


#define SetLabel(name,g1,g2,g3,g4,fontsize)\
{\
        name->setG(g1,g2,g3,g4);\
        QFont f=QFont();\
/*f.setFamily("OPPOSans R");*/\
        f.setPixelSize(fontsize);\
        name->setFont(f);\
}
#define SCALED(x,y) scaled(x,y,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)

#define NEW_LINEEDIT_MW(name,g1,g2,g3,g4,text,size) {\
        name=new QLineEdit(MW->centralWidget());\
        name->setG(rel(g1),rel(g2),rel(g3),rel(g4));\
        QFont f=QFont();\
/*f.setFamily("OPPOSans R");*/\
        f.setPixelSize(rel(size));\
        name->setFont(f);name->setText(text); name->show();}

class StartPage :public QObject, public MainWindow::Page
{
	Q_OBJECT
public:
	~StartPage() {}
	Game* G;
	MainWindow* MW;
	QPushButton* newgame;
	QPushButton* settings;
	QPushButton* quitgame;
	QPushButton* myicon;
	QPushButton* myname;
	QStringList Icons;
	void show_icons(int page)
	{
		{
			QFile file("res/icon/list.txt");
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			QTextStream inStream(&file);
			QString content = inStream.readAll();
			file.close();
			Icons = content.split('\n');
			//qDebug()<<Icons;
		}
		QDialog* dialog = new QDialog(MW->centralWidget());//设置头像
		dialog->setWindowTitle("设置头像");
		dialog->setModal(true);
		dialog->setFixedSize(400, 300); dialog->show();

		QLabel* bottom_label;
		NEW_LABEL_ALIGN(bottom_label, 180, 270, 40, 30, QN(page), 15, Center, dialog)
        QPushButton *icons[15];
        for (int id = 0; id < 15; id++)
        {

            {
                QPushButton*&ic=icons[id];
                if ((page - 1) * 15 + id < Icons.length())
                {NEW_BUTTON_IMAGE(ic, 50 + (id % 5) * 60, 50 + (id / 5) * 60, 40, 40, "res/icon/" + Icons[15 * (page - 1) + id], dialog);}
                else
                {NEW_BUTTON_IMAGE(ic, 50 + (id % 5) * 60, 50 + (id / 5) * 60, 40, 40, QSize(40,40), dialog); ic->hide();}
                ic->connect(ic, &QPushButton::clicked, this, [=]() {G->player->set_icon(Icons[15 * (page - 1) + id]); dialog->close(); MW->load_page(STARTPAGE); });
            }
        }
            {
                QPushButton* prev;
                NEW_BUTTON(prev, 10, 260, 30, 30, "<", 15, dialog);

                QPushButton* next;
                NEW_BUTTON(next, 360, 260, 30, 30, ">", 15, dialog);


                for(int targetid=0;targetid<15;targetid++)
                    {
                    QPushButton * target =icons[targetid];
                    connect(prev,&QPushButton::clicked,this,[=](){

                        int newpage=bottom_label->text().toInt()-1;
                        if((newpage-1)*15+targetid<Icons.length())
                            {
                            target->setIcon(QPixmap("res/icon/" + Icons[15 * (newpage - 1) + targetid]).SCALED(target->width(),target->height()) );
                            target->show();
                            }
                        else
                            target->hide();
                    });
                    }
                connect(prev,&QPushButton::clicked,this,[=](){

                        int newpage=bottom_label->text().toInt()-1;
                        bottom_label->setText(QN(newpage));
                        if(newpage>1) prev->show(); else prev->hide();
                        if(newpage<(Icons.length()-1)/15+1) next->show();else next->hide();
                    });
                    if(page>=1)prev->show();else prev->hide();

                for(int targetid=0;targetid<15;targetid++)
                {
                    QPushButton * target =icons[targetid];
                    connect(next,&QPushButton::clicked,this,[=](){

                        int newpage=bottom_label->text().toInt()+1;
                        if((newpage-1)*15+targetid<Icons.length())
                        {
                            target->setIcon(QPixmap("res/icon/" + Icons[15 * (newpage - 1) + targetid]).SCALED(target->width(),target->height()) );
                            target->show();
                        }
                        else
                            target->hide();
                    });
                }
                connect(next,&QPushButton::clicked,this,[=](){

                    int newpage=bottom_label->text().toInt()+1;
                    bottom_label->setText(QN(newpage));
                    if(newpage>1) prev->show(); else prev->hide();
                    if(newpage<(Icons.length()-1)/15+1) next->show();else next->hide();
                });
                if(page<(Icons.length()-1)/15+1)next->show();else next->hide();


            }

	}
	StartPage(MainWindow* mw) { MW = mw; G = mw->G; MW->page = this; }
	void load() override
	{
		G->player->load_name();

		NEW_BUTTON_IMAGE_MW(myicon, 665, 200, 70, 70, "res/icon/" + G->player->icon);
		NEW_BUTTON_MW(myname, 765, 200, 170, 70, G->player->name, 30);

		NEW_BUTTON_MW(newgame, 650, 300, 300, 150, "开始游戏", 30);
		connect(newgame, &QPushButton::clicked, MW, [=]() {MW->load_page(PVEPREP); });//qDebug()<<"xxxx\n";/*new_game()*/;});
		NEW_BUTTON_MW(quitgame, 650, 700, 300, 70, "退出游戏", 30);
		connect(quitgame, &QPushButton::clicked, MW, [=]() {qApp->quit();/*quit_game()*/; });

		connect(myname, &QPushButton::clicked, this, [=]() {
			QDialog* dialog = new QDialog(MW->centralWidget());//设置名字
			dialog->setWindowTitle("设置昵称");
			dialog->setModal(true);
			dialog->setFixedSize(160, 120); dialog->show();
			QFont f;
			/*f.setFamily("OPPOSans R");*/f.setP(15);
			QLabel* lab = new QLabel("你的名字:", dialog);
			SetLabel(lab, 5, 5, 100, 30, 16);
			QLineEdit* rename = new QLineEdit(G->player->name, dialog);
			rename->setFont(f);
			rename->setGeometry(5, 35, 110, 40);
			lab->show(); rename->show();

			QPushButton* sure = new QPushButton("确定", dialog);
			QPushButton* cancel = new QPushButton("取消", dialog);
			sure->setFont(f); cancel->setFont(f);
			sure->setGeometry(20, 80, 50, 30); cancel->setGeometry(90, 80, 50, 30);
			sure->show(); cancel->show();
			connect(sure, &QPushButton::clicked, this, [=]() {G->player->set_name(rename->text()); dialog->close(); MW->load_page(STARTPAGE); });
			connect(cancel, &QPushButton::clicked, this, [=]() {dialog->close(); dialog->deleteLater(); });
			});
		connect(myicon, &QPushButton::clicked, this, [=]() {show_icons(1); });

		NEW_BUTTON_MW(settings, 650, 500, 300, 70, "窗口设置", 30);
		connect(settings, &QPushButton::clicked, MW, [=]() {MW->load_page(SETTINGS);/*quit_game()*/; });
	}
};

class SettingsPage :public QObject, public MainWindow::Page
{
	//    Q_OBJECT
	~SettingsPage() {}
public:
	MainWindow* MW;
	QPushButton* button_600_375;
	QPushButton* button_800_500;
	QPushButton* button_1000_625;
	QPushButton* button_1200_750;
	QPushButton* button_1400_875;
	QPushButton* button_1600_1000;
	QPushButton* button_fullscreen;
	QPushButton* back;

	SettingsPage(MainWindow* mw) { MW = mw; MW->page = this; }
	void load() override
	{
#define SETTINGS_SCREENSIZE_BUTTON(A,B,C)\
        NEW_BUTTON_MW(button_ ##A##_##B ,650,C,300,50,#A +QString("x")+ #B ,rel(30));\
        connect(button_ ##A##_##B ,&QPushButton::clicked,MW,[=](){MW->showNormal();MW->resize(A,B);MW->load_page(STARTPAGE);});

		SETTINGS_SCREENSIZE_BUTTON(600, 375, 100);
		SETTINGS_SCREENSIZE_BUTTON(800, 500, 200);
		SETTINGS_SCREENSIZE_BUTTON(1000, 625, 300);
		SETTINGS_SCREENSIZE_BUTTON(1200, 750, 400);
		SETTINGS_SCREENSIZE_BUTTON(1400, 875, 500);
		SETTINGS_SCREENSIZE_BUTTON(1600, 1000, 600);
		NEW_BUTTON_MW(button_fullscreen, 650, 700, 300, 50, "全屏", rel(30));
		connect(button_fullscreen, &QPushButton::clicked, MW, [=]() {MW->showFullScreen(); MW->load_page(STARTPAGE); });

		NEW_BUTTON_MW(back, 650, 800, 300, 50, "返回", rel(30));
		connect(back, &QPushButton::clicked, MW, [=]() {MW->load_page(STARTPAGE); });
	}
};

class PvePrep :public MainWindow::Page
{
public:
	Game* G;
	MainWindow* MW;
	QLineEdit* name;
	QLabel* namehint;
	QLineEdit* seed;
	QLabel* seedhint;
	//QLabel *seedhint2;
	QPushButton* newgame_random, * newgame_selected, * back;

	QLabel* monster_icon, * monster_intro;

    static const int nMonster=7;
    static constexpr Game::MONSTER_ID monster_ids[nMonster + 1] = {
        Game::DEFAULT_MONSTER,
        Game::TWIN_HEAD,
        Game::MIMIC_CHEST,
        Game::CAT_BURGLAR,
        Game::SNOWMAN,
        Game::IRON_WALL,
        Game::STALL,
        Game::PUPPETEER,
    };
    QPushButton* e[nMonster + 1];
	PvePrep(MainWindow* mw) { MW = mw; G = MW->G; MW->page = this; }
	~PvePrep() {}
	void load() override
	{
		NEW_LINEEDIT_MW(name, 440, 200, 200, 60, G->player->name, 30);
		NEW_LINEEDIT_MW(seed, 440, 300, 200, 60, QN(G->random.getseed()), 30);
		NEW_LABEL_ALIGN_MW(namehint, 270, 210, 150, 30, "你的名字:", 30, Center);
		NEW_LABEL_ALIGN_MW(seedhint, 270, 310, 150, 30, "随机种子:", 30, Center);
		NEW_LABEL_ALIGN_MW(seedhint, 170, 370, 600, 60, "选择对手:若随机种子为空，则取当前时间", 30, Center);
		//  NEW_LABEL_ALIGN_MW(seedhint2,370,400,300,20,"若随机种子为空，则取当前时间",16,Center);
        NEW_BUTTON_MW(newgame_random,   220, 750, 160, 60, "开始新局", 30);
        NEW_BUTTON_MW(newgame_selected, 420, 750, 160, 60, "开始选局", 30);
        NEW_BUTTON_MW(back,             610, 750, 110, 60, "返回", 30);

        NEW_BUTTON_MW(e[1], 180, 450, 180, 70, "双头龙", 30);
        NEW_BUTTON_MW(e[2], 400, 450, 180, 70, "宝箱怪", 30);
        NEW_BUTTON_MW(e[3], 620, 450, 180, 70, "小贼猫", 30);
        NEW_BUTTON_MW(e[4], 180, 550, 180, 70, "雪人", 30);
        NEW_BUTTON_MW(e[5], 400, 550, 180, 70, "铁壁主宰", 30);
        NEW_BUTTON_MW(e[6], 620, 550, 180, 70, "saiwei", 30);
        NEW_BUTTON_MW(e[7], 180, 650, 180, 70, "系命傀儡师", 30);

		//怪物信息
		NEW_LABEL_IMAGE_MW(monster_icon, 1115, 100, 120, 120, G->monster->icon);
		QString str = G->monster->description();
        NEW_LABEL_ALIGN_MW(monster_intro, 840, 250, 670, 750, str, 30, Top | Qt::AlignLeft);
		monster_intro->setTextInteractionFlags(Qt::TextSelectableByMouse);


        for (int i = 1; i <= nMonster; i++)
            QPushButton::connect(e[i], &QPushButton::clicked, MW, [=]() {
                    if(MW->status==MainWindow::UISTATUS::NOT_WAITING)return;
                    MW->status=MainWindow::UISTATUS::NOT_WAITING;
            G->load_challenge(monster_ids[i]);
            for (int k = 1; k <= nMonster; k++)
			{
				e[k]->setStyleSheet(k == i ? "background-color:rgb(155,155,255)" : "");
			}
			monster_icon->setPixmap(QPixmap(G->monster->icon).scaled(monster_icon->size()));
			monster_intro->setText(G->monster->description());

            MW->status=MainWindow::UISTATUS::WAITING;
				});
		if (G->monster->id == 0)G->load_challenge(Game::TWIN_HEAD);
		emit e[G->monster->id]->clicked();

		QAbstractButton::connect(newgame_random, &QPushButton::clicked, MW, [=]() {
			G->random.setseed(time(NULL));
			G->player->name = name->text();
			//G->load_challenge(G->);//Game::SNOWMAN/*CAT_BURGLAR/*MIMIC_CHEST/*TWIN_HEAD*/);//设置冒险在这里改
			MW->load_page(PVEPAGE);
			G->RUN();
			});
		QAbstractButton::connect(newgame_selected, &QPushButton::clicked, MW, [=]() {
			if (seed->text().trimmed() == "")G->random.setseed(time(NULL)); else G->random.setseed(seed->text().trimmed().toInt());
			G->player->name = name->text();
			//G->load_challenge(G->);//Game::SNOWMAN/*TWIN_HEAD*/);//设置冒险在这里改
			MW->load_page(PVEPAGE);
			G->RUN();
            });

		QAbstractButton::connect(back, &QPushButton::clicked, MW, [=]() {MW->load_page(STARTPAGE); });

	}
};

class PvePage :public QObject, public MainWindow::Page
{
	Q_OBJECT
public:
	~PvePage() {}
	Game* G;
	MainWindow* MW;
	QPushButton* keys[20];
	QLabel* grid_background;

	QPushButton* monster_image;
	QLabel* monster_name;
	QLabel* monster_stats;
	QLabel* monster_extra;//额外内容
//    QLabel *monster_speak;
	QLabel* hero_image;
	QLabel* hero_name;
	QLabel* hero_stats;
	int nturnpieces;
	QPushButton* turn_pieces[MAX_XUANXIU];
	QPushButton* to_home;
	QPixmap gridimage;
	QLabel* turninfo;
	QLabel* turninfo2;

	QLabel* hero_stats2, * monster_stats2;

	QString cache_op = "";

	PvePage(MainWindow* mw) {
		//qDebug()<<"new pvepage";
		//op_cache="";
		MW = mw; G = MW->G; MW->page = this;
	}
	void load() override {
        using namespace GT;
		gridimage = QPixmap();
		G->player->grid.Make_image(gridimage);
		NEW_LABEL_IMAGE_MW(grid_background, 150, 100, 896, 896, gridimage);

		NEW_BUTTON_IMAGE_MW(monster_image, 1450, 100, 100, 100, G->monster->icon);
		NEW_LABEL_IMAGE_MW(hero_image, 1100, 100, 100, 100, "res/icon/" + G->player->icon);

		//QString const Unknown_stats = QString("? ? ?/? ? ?").replace(" ","");
		QString const Player_Initial = QString("0/150");
		QString const Dragon_Initial = QString("0/2000");
		NEW_LABEL_ALIGN_MW(monster_name, 1250, 300, 300, 50, G->monster->name, 40, Right);
		NEW_LABEL_ALIGN_MW(hero_name, 1100, 300, 300, 50, G->player->name, 40, Left);
		NEW_LABEL_ALIGN_MW(monster_stats, 1250, 350, 300, 50, Dragon_Initial, 30, Right);
		NEW_LABEL_ALIGN_MW(hero_stats, 1100, 350, 300, 50, Player_Initial, 30, Left);
		//NEW_LABEL_ALIGN_MW(monster_stats2,350,50,300,50,G->monster->name+": "+Dragon_Initial,12,Left);
		//NEW_LABEL_ALIGN_MW(hero_stats2,350,70,300,50,G->player->name+": "+Player_Initial,12,Left);
		NEW_LABEL_ALIGN_MW(monster_extra, 1100, 400, 450, 500, "", 30, HCenter);
		//NEW_LABEL_ALIGN_MW(turninfo2,250,50,200,50,"第0回合",20,Left);
        NEW_LABEL_ALIGN_MW(turninfo, 1200, 250, 300, 50, "第1/35回合", 40, HCenter);
		NEW_BUTTON_MW(to_home, 20, 20, 60, 60, "退出", 20);

		MainWindow::connect(to_home, &QPushButton::clicked, this, [=]() {G->remake(); MW->load_page(STARTPAGE); });//强制退出

		for (int i = 1; i < 20; i++)
		{
			NEW_BUTTON_MW(keys[i], 150 + GRID_X[i] * 128, 36 + GRID_Y[i] * 64, 128, 128, "", 15);//QString::number(i));//放块
			keys[i]->setFlat(true);
		}
		NEW_BUTTON_IMAGE_MW(keys[0], 150 + GRID_X[0] * 128 + 4, 36 + GRID_Y[0] * 64 + 4, 120, 120, "res/icon/diu312.png");//0号
		keys[0]->setIconSize(QSize(rel(120), rel(120)));

		for (int i = 0; i < 20; i++)
			MainWindow::connect(
				keys[i], &QPushButton::clicked, this, [=]() {
					monster_extra->setText("");
					if (G->Check_Operation(0, cache_op, QN(i)))
                        emit G->signal_new_operation(cache_op + "," + QN(i));//放块效果
				});
		//给按钮连上放块
		connect(monster_image, &QPushButton::clicked, this, [=]() {Show_info_in_new_dialog(); });

		connect(G, SIGNAL(Alert_monster(QString)), this, SLOT(Update_Battle_Log(QString)));//更新指令
		connect(G, &Game::signal_update_turn_piece, this, &PvePage::Update_Turn_Piece);
		connect(G, &Game::signal_before_turn, this, [=]() {Update_Turns(); });
        connect(G, SIGNAL(signal_player_change_stats(int,int)), this, SLOT(Update_Player_Stats(int,int)));
        connect(G, SIGNAL(signal_monster_change_stats(int,int,int)), this, SLOT(Update_Monster_Stats(int,int,int)));
		connect(G, &Game::signal_game_end, this, [=]() {Make_Summary(); });

		//    G->monster->id=Game::TWIN_HEAD;
	}

	void update() override {
		G->player->grid.Make_image(gridimage);
		grid_background->setPixmap(gridimage.SCALED(grid_background->width(), grid_background->height()));
		this->Update_Player_Stats(G->player->point(), G->player->health);
		this->Update_Monster_Stats(G->monster->point, G->monster->health, G->monster->armor);
	}
	void Make_Summary();
	//void Make_Detailed_Recap();
	void Clear_Turn_Piece() { for (int i = 0; i < nturnpieces; i++) { turn_pieces[i]->deleteLater(); turn_pieces[i] = NULL; }nturnpieces = 0; }
	void Set_Turn_Piece() {
		nturnpieces = G->pool->ncurrent;
		for (int i = 0; i < nturnpieces; i++)
		{
			NEW_BUTTON_IMAGE_MW(turn_pieces[i], 534 - 64 * (nturnpieces - 1) + i * 128, 36, 128, 128,
				QString("res/piece/card_") + G->pool->current[i].to_string() + ".png");
			if (nturnpieces >= 2)connect(turn_pieces[i], &QPushButton::clicked, this, [=]() {Toggle_Choose_Piece(i); });
		}
	}
	void Show_info_in_new_dialog();
public slots:
	void Toggle_Choose_Piece(int chosen)
	{
		cache_op = QN(chosen);
		for (int i = 0; i < nturnpieces; i++)
		{
			QPixmap map = QPixmap(QString("res/piece/card_") + G->pool->current[i].to_string() + ".png");
			if (i == chosen)
			{
				QPainter painter(&map);
				QPen pen; pen.setWidth(4); pen.setColor(Qt::red);
				painter.setPen(pen); painter.drawEllipse(10, 10, 44, 44);
			}
			turn_pieces[i]->setIcon(QIcon(map.scaled(turn_pieces[i]->size())));
		}
	}
	void Update_Battle_Log(QString s) { monster_extra->setText(monster_extra->text() + '\n' + s); }
	void Update_Turn_Piece() { Clear_Turn_Piece(); Set_Turn_Piece(); };
	void Update_Turns() {
		turninfo->setText("第" + QN(G->turn) + "/35回合"); //turninfo2->setText("第"+QN(G->turn)+"回合");
		cache_op = "";
	}
	void Update_Monster_Stats(int point, int health, int armor)
	{
		QString str = QString::number(point) + '/' + QString::number(health);
		if (armor > 0)str += "<" + QString::number(armor) + ">";
		monster_stats->setText(str);
		//monster_stats2->setText(G->monster->name+": "+str);
	}
	void Update_Player_Stats(int point, int health)
	{
		QString str = QString::number(point) + '/' + QString::number(health);
		hero_stats->setText(str);
		//hero_stats2->setText(G->player->name+": "+str);
	}
};

#endif // UI_H
