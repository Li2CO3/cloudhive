#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "game.h"
#include <QMainWindow>
#include <QTimer>
#include <QDialog>


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








class StartPage;class PvePage;class SettingsPage;

enum PageType{STARTPAGE=0,PVEPAGE=1,PVEPREP=2,SETTINGS=3};

class MainWindow;
extern MainWindow *TheWindow;
int rel(int size);


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    Page *page;

    enum UISTATUS{NOT_WAITING=0,WAITING=1};
    UISTATUS status;
    void load_page(PageType type);

    void Set_Piece(Game::Piece p, int place);
signals:
    void signal_made_operation();
};


#endif // MAINWINDOW_H
