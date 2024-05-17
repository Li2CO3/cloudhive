#ifndef STARTPAGE_H
#define STARTPAGE_H

#include "stdafx.h"
#include "../ui.h"
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
    QLabel *versioninfo;
    QPushButton *planebattle;

    QPushButton *numcomb;
    void show_icons(int page);
    StartPage(MainWindow* mw) { MW = mw; G = mw->G; MW->page = this; }
    void load() override;

};
#endif // STARTPAGE_H
