#ifndef PLANEBATTLEPAGE_H
#define PLANEBATTLEPAGE_H

#include "stdafx.h"
#include "../ui.h"
#include "../planebattlegame.h"
#include <QContextMenuEvent>
#include <QPixmap>

class PlaneCellButton :public QPushButton
{
    Q_OBJECT
public:
    PlaneCellButton(MainWindow *mw):QPushButton(mw->centralWidget()){MW=mw;
        connect(MW->PlaneG,&PlaneBattleGame::signal_update_grid,this,&PlaneCellButton::update_cell);}


    MainWindow *MW;
    PlaneBattleGame *PG;
    bool isbossgrid;
    int x;
    int y;
    bool ready=false;
    bool hover=false;

    PlaneCellButton *dual_button;

    void enterEvent(QEnterEvent *event) override
        {
        if(!ready)return;
        Q_UNUSED(event)
        hover=true;
            if(PG->boss_id==2)
        {dual_button->hover=true; dual_button->update_cell();}
            emit PG->signal_update_grid();
    }
    void leaveEvent(QEvent *event) override
        {
            if(!ready)return;
            Q_UNUSED(event)
            hover=false;
            if(PG->boss_id==2)
            {dual_button->hover=false; dual_button->update_cell();}
            emit PG->signal_update_grid();
        }

        bool this_place_is_striked()
        {
            if(isbossgrid)
            return PG->boss_grid[x][y].strike;
            else
            return PG->player_grid[x][y].strike;
        }
        bool this_place_has_plane()
        {
            for(int i=0;i<PG->n_playerplanes;i++)
            {
                auto t=PG->player_info[i];
                for(int j=0;j<9;j++)
                    if(t.centerx+PT::planeposition[t.direction][j][0]==x && t.centery+PT::planeposition[t.direction][j][1]==y) return true;
            }
            return false;
        }
        bool this_place_has_critical()
        {
            for(int i=0;i<PG->n_playerplanes;i++)
                if(PG->player_info[i].centerx==x && PG->player_info[i].centery==y) return true;
            return false;
        }
        bool this_place_has_RDCenter()
        {
            return false;
        }
        void Make_Put_Plane_Menu()
        {
            {

                QMenu menu;
                auto up = menu.addAction("上",PG,[=](){PG->Player_put_plane(x,y,PT::Up);});
                               if(!PG->player_can_put_plane(PT::Plane_info(PT::PlaneType_Normal,PT::PlaneDir_U,x,y)))
                        up->setEnabled(false);
                auto down = menu.addAction("下",PG,[=](){PG->Player_put_plane(x,y,PT::Down);});
                               if(!PG->player_can_put_plane(PT::Plane_info(PT::PlaneType_Normal,PT::PlaneDir_D,x,y)))
                        down->setEnabled(false);
                                auto left = menu.addAction("左",PG,[=](){PG->Player_put_plane(x,y,PT::Left);});
                                if(!PG->player_can_put_plane(PT::Plane_info(PT::PlaneType_Normal,PT::PlaneDir_L,x,y)))
                        left->setEnabled(false);
                                auto right = menu.addAction("右",PG,[=](){PG->Player_put_plane(x,y,PT::Right);});
                                if(!PG->player_can_put_plane(PT::Plane_info(PT::PlaneType_Normal,PT::PlaneDir_R,x,y)))
                        right->setEnabled(false);

                //以阻塞方式显示菜单，参数可指示菜单显示位置，另外该函数可返回单击选中项
                auto t = menu.exec(QCursor::pos());//a QAction
                                //qDebug()<<t;
            }
        }

        void Make_Mark_Plane_Menu()
        {
            {

                QMenu menu;
                auto up = menu.addAction("上",PG,[=](){PG->Mark_plane(x,y,PT::Up);});

                auto down = menu.addAction("下",PG,[=](){PG->Mark_plane(x,y,PT::Down);});

                                auto left = menu.addAction("左",PG,[=](){PG->Mark_plane(x,y,PT::Left);});

                                auto right = menu.addAction("右",PG,[=](){PG->Mark_plane(x,y,PT::Right);});


                //以阻塞方式显示菜单，参数可指示菜单显示位置，另外该函数可返回单击选中项
                auto t = menu.exec(QCursor::pos());//a QAction
                                //qDebug()<<t;
            }
        }

        void mousePressEvent(QMouseEvent *event) override
        {
            // 调用基类的mousePressEvent以处理其他事件
            QPushButton::mousePressEvent(event);
            if (event->button() == Qt::LeftButton)
            {
                                // 单击时的操作
                                Leftclicked();
            }
            else if (event->button() == Qt::RightButton)
            {
                                // 右键点击时的操作
                                Rightclicked();
            }

        }
        void Leftclicked()
        {
            if(isbossgrid)
            {
                if(PG->game_status==PT::Put_Planes)
                                {return;}
                if(PG->game_status==PT::Combat_Planes)
                {
                        if(this_place_is_striked())return;
                        emit PG->signal_player_move(x,y,5);
                            return;
                }
                if(PG->game_status==PT::End_Planes)
                {return;}
            }
            else
            {
                if(PG->game_status==PT::Put_Planes)
                {if(this_place_has_critical())
                                PG->Player_remove_plane(x,y);
                            else if(this_place_has_plane())
                            {}
                            else
                                Make_Put_Plane_Menu();
                }
                if(PG->game_status==PT::Combat_Planes)
                {return;//mark
                }
                if(PG->game_status==PT::End_Planes)
                {return;}
            }

        }
        void Rightclicked()
        {
            if(isbossgrid)
            {
                if(PG->game_status==PT::Put_Planes)
                {}
                if(PG->game_status==PT::Combat_Planes)
                {
                            Make_Mark_Plane_Menu();

                }
                if(PG->game_status==PT::End_Planes)
                {}
            }
            else
            {
                if(PG->game_status==PT::Put_Planes)
                {Make_Put_Plane_Menu();}
                if(PG->game_status==PT::Combat_Planes)
                {}
                if(PG->game_status==PT::End_Planes)
                {}
            }
        }
        void update_cell()
        {
            auto c=(isbossgrid?PG->boss_grid:PG->player_grid)[x][y];
            setIcon(cellpic(PG->cell_using_status(x,y,isbossgrid),c.strike,c.strike_turn==PG->round,PT::Cell_Mark_type(0),45,45,(!isbossgrid) || PG->game_status==PT::End_Planes));
            setIconSize(size());

        }
        QPixmap cellpic_game(PT::Cell_Plane_type real_t,
                               PT::Cell_Strike_type strike_t,bool strike_new,
                               PT::Cell_Mark_type mark_t,
                             int pic_x, int pic_y) {return cellpic(real_t, strike_t,strike_new, mark_t, pic_x, pic_y, true);}


        QPixmap cellpic_end(PT::Cell_Plane_type real_t,
                             PT::Cell_Strike_type strike_t,bool strike_new,
                             PT::Cell_Mark_type mark_t,
                             int pic_x, int pic_y) {return cellpic(real_t, strike_t,strike_new, mark_t, pic_x, pic_y, false);}

        QPixmap cellpic(PT::Cell_Plane_type real_t,
                          PT::Cell_Strike_type strike_t,bool strike_new,
                          PT::Cell_Mark_type mark_t,
                        int pic_x, int pic_y, bool see_through);

};

#define NEW_PLANEBUTTON_MW(name,g1,g2,g3,g4,text,fontsize,parent)\
{name=new PlaneCellButton(parent);\
name->setG(rel(g1),rel(g2),rel(g3),rel(g4));\
            name->setText(text);\
            QFont f;\
            f.setP(rel(fontsize));\
            /*f.setStyleStrategy(QFont::PreferAntialias);*/\
            name->setFont(f);\
            name->show();}

class PlaneBattlePage :public MainWindow::Page
{
    MainWindow *MW;
    PlaneBattleGame *PG;

    PlaneCellButton *playergrid[14][14];
    PlaneCellButton *bossgrid[14][14];
    QPushButton * back;
    QPushButton * confirmput;
    QLabel *challenger;
    QPushButton *challengericon;
    QLabel *Boss;
    QPushButton *Bossicon;
    QLabel *bulletin;
    QPushButton *ClearMark;

    QLabel *turninfo;
    QLabel *remainheadinfo;
    QLabel *remainmissileinfo;
    QLabel *nukewarning;
public:


    PlaneBattlePage(MainWindow *mw){
        MW = mw;
            PG=MW->PlaneG; MW->page = this;}

    void load() override;
    void update() override;

};
#endif // PLANEBATTLEPAGE_H
