#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "stdafx.h"
#include "../ui.h"
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
#endif // SETTINGSPAGE_H
