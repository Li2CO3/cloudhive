#ifndef PLANEBATTLEPREPPAGE_H
#define PLANEBATTLEPREPPAGE_H

#include "stdafx.h"
#include "../ui.h"
#include "../planebattlegame.h"
class PlaneBattlePrepPage :public MainWindow::Page
{
    MainWindow *MW;
    Game *G;

    int pref_choices[5];

    QPushButton *bosskey[2];
    QPushButton *overlapkey[2];
    QPushButton *criticalkey[3];
    QPushButton *missilekey[10];
    QPushButton *inspectkey[32];
    QPushButton *back;
    QPushButton *play;
public:
    static QString const notes[5],bossname[2],overlaprule[2],criticalrule[3];
    PlaneBattlePrepPage(MainWindow *mw){ MW = mw; G = MW->G; MW->page = this; }
    ~PlaneBattlePrepPage(){}
    void load() override
    {
        for(int i=0;i<2;i++)
        {
            NEW_BUTTON_MW(bosskey[i],70+200*i,50,150,70,bossname[i],20);
            NEW_BUTTON_MW(overlapkey[i],70+200*i,150,150,70,overlaprule[i],20);
        }
        for(int i=0;i<3;i++)
            {NEW_BUTTON_MW(criticalkey[i],70+200*i,250,150,70,criticalrule[i],20);}
        for(int i=0;i<10;i++)
            {NEW_BUTTON_MW(missilekey[i],70+100*i,350,70,70,QN(i+1),20);}
        for(int i=0;i<32;i++)
            {NEW_BUTTON_MW(inspectkey[i],70+100*(i%8),450+100*(i/8),70,70,
                          (i==31?QString("随机"):QN(i)),20);}


            KEY_SELECT_MORE(bosskey,0,2,pref_choices[0]=i+1)
            KEY_SELECT_MORE(overlapkey,0,2,pref_choices[1]=i)
            KEY_SELECT_MORE(criticalkey,0,3,pref_choices[2]=i)
            KEY_SELECT_MORE(missilekey,0,10,pref_choices[3]=i+1)
            KEY_SELECT_MORE(inspectkey,0,32,pref_choices[4]=(i==31?100:i))

            emit bosskey[0]->clicked();
            emit overlapkey[0]->clicked();
            emit criticalkey[0]->clicked();
            emit missilekey[2]->clicked();
            emit inspectkey[10]->clicked();

            NEW_BUTTON_MW(play,800,80,70,70,"开始",20);
            NEW_BUTTON_MW(back,800,180,70,70,"返回",20);

            QAbstractButton::connect(play,&QPushButton::clicked, MW,[=](){
                if(MW->PlaneG)delete MW->PlaneG;
                MW->PlaneG=new PlaneBattleGame(MW);
                MW->PlaneG->startPlaneBattle(pref_choices[0],pref_choices[1],pref_choices[2],pref_choices[3],pref_choices[4],"");
                MW->load_page(PLANEBATTLEPAGE);
            });
            QAbstractButton::connect(back,&QPushButton::clicked, MW,[=](){
                MW->load_page(STARTPAGE);
            });
    }


};


const QString PlaneBattlePrepPage::notes[5]={"BOSS","重叠","要害","连发",""};
const QString PlaneBattlePrepPage::bossname[2]={"[1]无限火力","[2]核平铀好"};
const QString PlaneBattlePrepPage::overlaprule[2]={"不允许","允许"};
const QString PlaneBattlePrepPage::criticalrule[3]={"显示","不显示","仅显示首次"};
#endif // PLANEBATTLEPREPPAGE_H
