#ifndef NUMCOMBPREPPAGE_H
#define NUMCOMBPREPPAGE_H

#include "stdafx.h"
#include "../ui.h"
#include "monster.h"
#include "../utils/comb_assembler.hpp"

class NumcombPrepPage :public MainWindow::Page
{
public:
    Game* G;
    MainWindow* MW;
    QLineEdit* name;
    QLabel* namehint;
    QLineEdit* seed;
    QLabel* seedhint;
    QPushButton *HUIMIE, *BOOM ;
    //QLabel *seedhint2;
    QPushButton* newgame_random, * newgame_selected, * back;



    NumcombPrepPage(MainWindow* mw) { MW = mw; G = MW->G; MW->page = this; }
    ~NumcombPrepPage() {}
    void load() override
    {


        NEW_LINEEDIT_MW(name, 740, 300, 200, 60, G->player()->name, 30);
        NEW_LINEEDIT_MW(seed, 740, 400, 200, 60, G->random->getseed(), 30);
        NEW_LABEL_ALIGN_MW(namehint, 570, 310, 150, 30, "你的名字:", 30, Center);
        NEW_LABEL_ALIGN_MW(seedhint, 570, 410, 150, 30, "随机种子:", 30, Center);
        NEW_BUTTON_MW(newgame_random,   520,650, 160, 60, "开始新局", 30);
        NEW_BUTTON_MW(newgame_selected, 720,650, 160, 60, "开始选局", 30);
        NEW_BUTTON_MW(back,             920,650, 110, 60, "返回", 30);

        NEW_BUTTON_MW(HUIMIE, 1240,920,160,60,"毁灭者种子", 30);
        NEW_BUTTON_MW(BOOM,1420,920,160,60,"毁灭者自选",30);
                      QAbstractButton::connect(newgame_random, &QPushButton::clicked, MW, [=]() {

                          G->player()->name = name->text();
                          MW->load_page(NUMCOMBPAGE);
                          G->Start(QN(time(NULL)));
                      });
                      QAbstractButton::connect(newgame_selected, &QPushButton::clicked, MW, [=]() {
                          QString seedstr;
                          if (seed->text().trimmed() == "")seedstr=(QN(time(NULL))); else seedstr=(seed->text().simplified());
                          G->player()->name = name->text();
                          MW->load_page(NUMCOMBPAGE);
                          G->Start(seedstr);
                      });

                      QAbstractButton::connect(back, &QPushButton::clicked, MW, [=]() {MW->load_page(STARTPAGE); });

                      QAbstractButton::connect(HUIMIE, &QPushButton::clicked, MW,[=](){
                          QDialog *dialog =new QDialog(MW);
                          Comb_Assembler::Make_Assemble_Page("seed "+seed->text().simplified(),dialog);
                          QObject::connect(dialog,&QDialog::rejected, dialog,&QDialog::close);
                      });
                      QAbstractButton::connect(BOOM, &QPushButton::clicked, MW,[=](){
                          MW->load_page(ASSEMBLECRAFTPAGE);});
    }
};
#endif // NUMCOMBPREPPAGE_H
