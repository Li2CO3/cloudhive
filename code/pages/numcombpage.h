#ifndef NUMCOMBPAGE_H
#define NUMCOMBPAGE_H
#include "stdafx.h"
#include "../ui.h"
#include "utils/gametools.h"
#include "cardpool.h"
#include "monster.h"
#include "QDir"

class NumcombPage :public QObject, public MainWindow::Page
{
    Q_OBJECT
public:
    ~NumcombPage() {}
    static const int SHIFT_X=50;
    static const int SHIFT_Y=0;
    Game* G;
    MainWindow* MW;
    QPushButton* keys[46];
    QLabel* grid_background[NUMCOMB_MAX_PLAYER];

    //    QLabel *monster_speak;
    int const nplayer=1;
    QLabel* hero_image[NUMCOMB_MAX_PLAYER];
    QLabel* hero_name[NUMCOMB_MAX_PLAYER];
    QLabel* hero_point[NUMCOMB_MAX_PLAYER];

    QPushButton* turn_piece=NULL;
    QPushButton* to_home;
    QPixmap gridimage[NUMCOMB_MAX_PLAYER];
    QLabel* turninfo;

    QString cache_op = "";

    NumcombPage(MainWindow* mw) {
        //qDebug()<<"new pvepage";
        //op_cache="";
        MW = mw; G = MW->G; MW->page = this;
    }
    void load() override;


    void update() override {
        for(int i=0;i<nplayer;i++)
        {
            G->player(i)->grid.Make_image(gridimage[i],GT::NUM_GRID);
            grid_background[i]->setPixmap(gridimage[i].SCALED(grid_background[i]->width(), grid_background[i]->height()));
            this->Update_Player_Stats(G->player()->stat_string(),i);
        }
    }
    void Make_Summary();
    //void Make_Detailed_Recap();
    void Clear_Turn_Piece() { if(turn_piece)turn_piece->deleteLater();turn_piece = NULL; }
    void Set_Turn_Piece() {
        {
            NEW_BUTTON_IMAGE_MW(turn_piece, 534 +SHIFT_X - 64 * (0) + 0 * 128, 36+SHIFT_Y, 128, 128, G->pool()->current[0].image());
        }
    }
    void Show_info_in_new_dialog();
public slots:

    void Update_Turn_Piece() { Clear_Turn_Piece(); Set_Turn_Piece(); };
    void Update_Turns() {        turninfo->setText("第" + QN(G->turn) + "/20回合");}
    void Update_Player_Stats(QString str, int ply)//int point, int health)
    {
        //QString str = QString::number(point) + '/' + QString::number(health);
        hero_point[ply]->setText(str);
        //hero_stats2->setText(G->player()->name+": "+str);
    }
};
#endif // NUMCOMBPAGE_H
