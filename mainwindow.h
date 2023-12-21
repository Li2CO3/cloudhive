#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "game.h"
#include <QMainWindow>


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
        virtual void deleteLater(){delete this;}
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
