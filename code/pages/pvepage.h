#ifndef PVEPAGE_H
#define PVEPAGE_H

#include "stdafx.h"
#include "../ui.h"
#include "utils/gametools.h"
#include "cardpool.h"
#include "monster.h"
#include "QDir"

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
        MW->current_monster_id=MW->G->monster()->id;
    }
    void load() override;


    void update() override {
        G->player()->grid.Make_image(gridimage,GT::CLOUD_GRID);
        grid_background->setPixmap(gridimage.SCALED(grid_background->width(), grid_background->height()));
        this->Update_Player_Stats(G->player()->stat_string());
        this->Update_Monster_Stats(G->monster()->stat_string());
    }
    void Make_Summary();
    //void Make_Detailed_Recap();
    void Clear_Turn_Piece() { for (int i = 0; i < nturnpieces; i++) { turn_pieces[i]->deleteLater(); turn_pieces[i] = NULL; }nturnpieces = 0; }
    void Set_Turn_Piece() {
        nturnpieces = G->pool()->ncurrent;
        for (int i = 0; i < nturnpieces; i++)
        {
            NEW_BUTTON_IMAGE_MW(turn_pieces[i], 534 - 64 * (nturnpieces - 1) + i * 128, 36, 128, 128, G->pool()->current[i].image());
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
            QPixmap map = QPixmap(G->pool()->current[i].image());
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
    void Update_Monster_Stats (QString str)//(int point, int health, int armor)
    {
        //QString str = QString::number(point) + '/' + QString::number(health);
        //if (armor > 0)str += "<" + QString::number(armor) + ">";
        monster_stats->setText(str);
        //monster_stats2->setText(G->monster()->name+": "+str);
    }
    void Update_Player_Stats(QString str, int ply=0)//int point, int health)
    {
        Q_UNUSED(ply)
        //QString str = QString::number(point) + '/' + QString::number(health);
        hero_stats->setText(str);
        //hero_stats2->setText(G->player()->name+": "+str);
    }
};
#endif // PVEPAGE_H
