#ifndef PVEPREPPAGE_H
#define PVEPREPPAGE_H

#include "stdafx.h"
#include "../ui.h"
#include "monster.h"

class PvePrepPage :public MainWindow::Page
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

    static const int nMonster=9
#ifdef NewYear2024NianEasy
        +1
#endif
        ;



    QPushButton* e[nMonster + 1];
    PvePrepPage(MainWindow* mw) { MW = mw; G = MW->G; MW->page = this; }
    ~PvePrepPage() {}
    void load() override
    {
        if (G->monster->id == 0)G->load_challenge(GT::TWIN_HEAD);

        NEW_LINEEDIT_MW(name, 440, 200, 200, 60, G->player->name, 30);
        NEW_LINEEDIT_MW(seed, 440, 300, 200, 60, QN(G->random.getseed()), 30);
        NEW_LABEL_ALIGN_MW(namehint, 270, 210, 150, 30, "你的名字:", 30, Center);
        NEW_LABEL_ALIGN_MW(seedhint, 270, 310, 150, 30, "随机种子:", 30, Center);
        NEW_LABEL_ALIGN_MW(seedhint, 170, 370, 600, 60, "选择对手:", 30, Center);
        NEW_BUTTON_MW(newgame_random,   220, 550+(nMonster-1)/3*100, 160, 60, "开始新局", 30);
        NEW_BUTTON_MW(newgame_selected, 420, 550+(nMonster-1)/3*100, 160, 60, "开始选局", 30);
        NEW_BUTTON_MW(back,             610, 550+(nMonster-1)/3*100, 110, 60, "返回", 30);


            GT::MONSTER_ID monster_ids[nMonster + 1] = {
                GT::DEFAULT_MONSTER,
                GT::TWIN_HEAD,
                GT::MIMIC_CHEST,
                GT::CAT_BURGLAR,
                GT::SNOWMAN,
                GT::IRON_WALL,
                GT::STALL,
                GT::PUPPETEER,
                GT::RAINBOW_GIRAFFE,
                GT::NIAN_MONSTER,
#ifdef NewYear2024NianEasy
                GT::NIAN_EASY,
#endif
            };
            QString monster_key_text[nMonster + 1] = {
                "",
                "双头龙",
                "宝箱怪",
                "小贼猫",
                "雪人",
                "铁壁主宰",
                "saiwei",
                "系命傀儡师",
                "彩虹长颈鹿",
                "年兽",
#ifdef NewYear2024NianEasy
                "赖子年兽",
#endif
            };
        for(int key_id=1;key_id<nMonster+1;key_id++)
            NEW_BUTTON_MW(e[key_id], 180+(key_id-1)%3*220, 450+(key_id-1)/3*100, 180, 70, monster_key_text[key_id], 30);


        //怪物信息
        NEW_LABEL_IMAGE_MW(monster_icon, 1115, 100, 120, 120, G->monster->icon);
        QString str = G->monster->description();
        NEW_LABEL_ALIGN_MW(monster_intro, 840, 250, 690, 750, str, 30, Top | Qt::AlignLeft);
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
        {
            int current_monster_button=nMonster;
            for(;current_monster_button>0;current_monster_button--)
                if(monster_ids[current_monster_button]==G->monster->id)break;
        emit e[current_monster_button]->clicked();
        }
        QAbstractButton::connect(newgame_random, &QPushButton::clicked, MW, [=]() {
            G->random.setseed(time(NULL));
            G->player->name = name->text();
            //G->load_challenge(G->);//Game::SNOWMAN/*CAT_BURGLAR/*MIMIC_CHEST/*TWIN_HEAD*/);//设置冒险在这里改
            MW->load_page(PVEPAGE);
            G->RUN(Game::MODE_PVE);
            });
        QAbstractButton::connect(newgame_selected, &QPushButton::clicked, MW, [=]() {
            if (seed->text().trimmed() == "")G->random.setseed(time(NULL)); else G->random.setseed(seed->text().trimmed().toInt());
            G->player->name = name->text();
            //G->load_challenge(G->);//Game::SNOWMAN/*TWIN_HEAD*/);//设置冒险在这里改
            MW->load_page(PVEPAGE);
            G->RUN(Game::MODE_PVE);
            });

        QAbstractButton::connect(back, &QPushButton::clicked, MW, [=]() {MW->load_page(STARTPAGE); });

    }
};
#endif // PVEPREPPAGE_H
