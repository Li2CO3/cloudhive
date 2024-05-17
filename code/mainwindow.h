﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QDialog>

#include "game.h"

#define setG(g1,g2,g3,g4) setGeometry((g1),(g2),(g3),(g4))
#define setP(g) setPixelSize((g))
#define MC() MW->centralWidget()

#define rel(x) int((x) * MW->width()/1600)
#define font_rel(x) (rel(x)+(((rel(x)==11)||(rel(x)==14)||(rel(x)==16))?-1:0))//11 14 15 16的显示效果不好。600*375会变成10号。800*500的画面会是15。换字体？

#define NEW_BUTTON_MW(name,g1,g2,g3,g4,text,fontsize) NEW_BUTTON(name,rel(g1),rel(g2),rel(g3),rel(g4),text,font_rel(fontsize),MC())

#define NEW_BUTTON(name,g1,g2,g3,g4,text,fontsize,parent)\
{name=new QPushButton(parent);\
        name->setG(g1,g2,g3,g4);\
        name->setText(text);\
        QFont f;\
        f.setP(fontsize);\
        /*f.setStyleStrategy(QFont::PreferAntialias);*/\
        name->setFont(f);\
        name->show();}

#define NEW_LABEL_MW(name,g1,g2,g3,g4,text,fontsize)  NEW_LABEL(name,rel(g1),rel(g2),rel(g3),rel(g4),text,font_rel(fontsize),MC())

#define NEW_LABEL(name,g1,g2,g3,g4,text,fontsize,parent)\
{name=new QLabel(parent);\
        name->setG(g1,g2,g3,g4);\
        name->setText(text);\
        name->setWordWrap(true);\
        QFont f;\
        /*f.setStyleStrategy(QFont::PreferAntialias);*/\
        f.setP(fontsize);\
        name->setFont(f);\
        name->show();}

#define NEW_LABEL_ALIGN_MW(name,g1,g2,g3,g4,text,fontsize,align) NEW_LABEL_ALIGN(name,rel(g1),rel(g2),rel(g3),rel(g4),text,font_rel(fontsize),align,MC())

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
        f.setPixelSize(fontsize);\
        name->setFont(f);\
}
#define SCALED(x,y) scaled(x,y,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)

#define NEW_LINEEDIT_MW(name,g1,g2,g3,g4,text,size) {\
name=new QLineEdit(MW->centralWidget());\
    name->setG(rel(g1),rel(g2),rel(g3),rel(g4));\
    QFont f=QFont();\
    f.setPixelSize(font_rel(size));\
    name->setFont(f);name->setText(text); name->show();}

#define KEY_SELECT_MORE(seq,start,n,other) {\
for(int i=start;i<start+n;i++)\
QPushButton::connect(seq[i], &QPushButton::clicked, MW, [=]() {\
for (int k = start; k < start+n; k++)\
{\
    seq[k]->setStyleSheet(k == i ? "background-color:rgb(155,155,255)" : "");\
}\
other;\
});\
}
#define KEY_SELECT(seq,start,n) KEY_SELECT_MORE(seq,start,n,{})


class StartPage;class PvePage;class SettingsPage;

enum PageType{STARTPAGE=0,PVEPAGE=1,PVEPREPPAGE=2,SETTINGSPAGE=3,NUMCOMBPAGE=4, NUMCOMBPREPPAGE=5, ASSEMBLECRAFTPAGE=6,
                PLANEBATTLEPREPPAGE=7, PLANEBATTLEPAGE=8};

class MainWindow;
class PlaneBattleGame;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    QString name;
    QString icon;
    GT::MONSTER_ID current_monster_id=GT::TWIN_HEAD;


    void set_name(QString name);
    void set_icon(QString icon);
    class Page
    {
    public:
        virtual void load(){};
        virtual void update(){}
        virtual ~Page(){}
        //virtual void deleteLater(){delete this;}
    };

    void clear_all_object(){
            QWidget *newcenter = new QWidget();
        newcenter->setGeometry(centralWidget()->geometry());
        centralWidget()->deleteLater();setCentralWidget(newcenter);
        newcenter->show();
        page=NULL;//page已经在上面被删了
    }
    Game *G;
    PlaneBattleGame *PlaneG=nullptr;
    Page *page;

    enum UISTATUS{NOT_WAITING=0,WAITING=1};
    UISTATUS status;
    void load_page(PageType type);

    void Set_Piece(Piece p, int place);
    void load_name();//看res/game/user.txt设置名字
signals:
    void signal_made_operation();
};


#endif // MAINWINDOW_H
