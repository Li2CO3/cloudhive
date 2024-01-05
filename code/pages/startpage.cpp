#include "startpage.h"
#include "../utils/comb_assembler.hpp"
void StartPage::show_icons(int page)
{
    int const ICONS_PER_PAGE=15;

    {
        QFile file("res/icon/list.txt");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream inStream(&file);
        QString content = inStream.readAll();
        file.close();
        Icons = content.split('\n');
        //qDebug()<<Icons;
    }
    QDialog* dialog = new QDialog(MW->centralWidget());//设置头像
    dialog->setWindowTitle("设置头像");
    dialog->setModal(true);
    dialog->setFixedSize(400, 300); dialog->show();

    QLabel* bottom_label;
    NEW_LABEL_ALIGN(bottom_label, 180, 270, 40, 30, QN(page), 15, Center, dialog)
    QPushButton *icons[ICONS_PER_PAGE];
    for (int id = 0; id < ICONS_PER_PAGE; id++)
    {

        {
            QPushButton*&ic=icons[id];
            if ((page - 1) * ICONS_PER_PAGE + id < Icons.length())
                {NEW_BUTTON_IMAGE(ic, 50 + (id % 5) * 60, 50 + (id / 5) * 60, 40, 40, "res/icon/" + Icons[ICONS_PER_PAGE * (page - 1) + id], dialog);}//图标按钮
            else
                {NEW_BUTTON_IMAGE(ic, 50 + (id % 5) * 60, 50 + (id / 5) * 60, 40, 40, QSize(40,40), dialog); ic->hide();}
            ic->connect(ic, &QPushButton::clicked, this, [=]() {
                    MW->set_icon(Icons[ICONS_PER_PAGE * (bottom_label->text().toInt() - 1) + id]);
                    dialog->close(); MW->load_page(STARTPAGE);
                });
        }
    }
    {
        QPushButton* prev;
        NEW_BUTTON(prev, 10, 260, 30, 30, "<", 15, dialog);
        prev->hide();
        QPushButton* next;
        NEW_BUTTON(next, 360, 260, 30, 30, ">", 15, dialog);
        if(Icons.length()<=ICONS_PER_PAGE)next->hide();

        for(int targetid=0;targetid<ICONS_PER_PAGE;targetid++)
        {
            QPushButton * target =icons[targetid];
            connect(prev,&QPushButton::clicked,this,[=](){

                int newpage=bottom_label->text().toInt()-1;
                if((newpage-1)*ICONS_PER_PAGE+targetid<Icons.length())
                {
                    target->setIcon(QPixmap("res/icon/" + Icons[ICONS_PER_PAGE * (newpage - 1) + targetid]).SCALED(target->width(),target->height()) );
                    target->show();
                }
                else
                    target->hide();
            });
        }

        {
            connect(prev,&QPushButton::clicked,this,[=](){

                int newpage=bottom_label->text().toInt()-1;
                bottom_label->setText(QN(newpage));
                if(newpage>1) prev->show(); else prev->hide();
                if(newpage<(Icons.length()-1)/ICONS_PER_PAGE+1) next->show();else next->hide();
            });
        }
        if(page>1)prev->show();
            else prev->hide();


        {
            for(int targetid=0;targetid<ICONS_PER_PAGE;targetid++)
            {
                QPushButton * target =icons[targetid];
                connect(next,&QPushButton::clicked,this,[=](){

                    int newpage=bottom_label->text().toInt()+1;
                    if((newpage-1)*ICONS_PER_PAGE+targetid<Icons.length())
                    {
                        target->setIcon(QPixmap("res/icon/" + Icons[ICONS_PER_PAGE * (newpage - 1) + targetid]).SCALED(target->width(),target->height()) );
                        target->show();
                    }
                    else
                        target->hide();
                });
            }
            connect(next,&QPushButton::clicked,this,[=](){

                int newpage=bottom_label->text().toInt()+1;
                bottom_label->setText(QN(newpage));
                if(newpage>1) prev->show(); else prev->hide();
                if(newpage<(Icons.length()-1)/ICONS_PER_PAGE+1) next->show();else next->hide();
            });
        }
        if(page<(Icons.length()-1)/ICONS_PER_PAGE+1)next->show();else next->hide();


    }

}
void StartPage::load()
    {

    /*    for(int i=1;i<=30;i++)
            {
        QLabel *lab=new QLabel(MW);
        lab->setText("0123456789");
        lab->setGeometry((i-1)%3*160,(i-1)/3*40,160,40);
        QFont f;
        f.setPixelSize(i);
        f.setStyleStrategy(QFont::NoAntialias);
        lab->setFont(f);lab->show();
        lab->lower();
            }
    */
        MW->load_name();
        NEW_LABEL_ALIGN_MW(versioninfo,1280,20,300,50,"版本:"+__VERSION,30,Top |Qt::AlignRight);

        NEW_BUTTON_IMAGE_MW(myicon, 665, 200, 70, 70, "res/icon/" + MW->icon);
        NEW_BUTTON_MW(myname, 765, 200, 170, 70, MW->name, 30);

        NEW_BUTTON_MW(newgame, 650, 300, 300, 150, "开始游戏", 30);
        connect(newgame, &QPushButton::clicked, MW, [=]() {G->remake(GT::PVEGAME);MW->load_page(PVEPREPPAGE); });//qDebug()<<"xxxx\n";/*new_game()*/;});

        NEW_BUTTON_MW(quitgame, 650, 700, 300, 70, "退出游戏", 30);
        connect(quitgame, &QPushButton::clicked, MW, [=]() {qApp->quit();/*quit_game()*/; });

        connect(myname, &QPushButton::clicked, this, [=]() {
            QDialog* dialog = new QDialog(MW->centralWidget());//设置名字
            dialog->setWindowTitle("设置昵称");
            dialog->setModal(true);
            dialog->setFixedSize(160, 120); dialog->show();
            QFont f;
            /*f.setFamily("");*/f.setP(17);
            QLabel* lab = new QLabel("你的名字:", dialog);
            SetLabel(lab, 5, 5, 100, 30, 17);
            QLineEdit* rename = new QLineEdit(MW->name, dialog);
            rename->setFont(f);
            rename->setGeometry(5, 35, 110, 40);
            lab->show(); rename->show();

            QPushButton* sure = new QPushButton("确定", dialog);
            QPushButton* cancel = new QPushButton("取消", dialog);
            sure->setFont(f); cancel->setFont(f);
            sure->setGeometry(20, 80, 50, 30); cancel->setGeometry(90, 80, 50, 30);
            sure->show(); cancel->show();
            connect(sure, &QPushButton::clicked, this, [=]() {MW->set_name(rename->text()); dialog->close(); MW->load_page(STARTPAGE); });
            connect(cancel, &QPushButton::clicked, this, [=]() {dialog->close(); dialog->deleteLater(); });
            });
        connect(myicon, &QPushButton::clicked, this, [=]() {show_icons(1); });

        NEW_BUTTON_MW(settings, 650, 500, 300, 70, "窗口设置", 30);
        connect(settings, &QPushButton::clicked, MW, [=]() {MW->load_page(SETTINGSPAGE);/*quit_game()*/; });

        NEW_BUTTON_MW(numcomb, 650, 600, 300,70,"数字蜂巢", 30);
        QAbstractButton::connect(numcomb, &QPushButton::clicked, MW, [=]() {
            QString seedstr="bored";
            MW->G->remake(GT::GAMETYPE::NUMCOMBGAME);
            //if (seed->text().trimmed() == "")seedstr=(QN(time(NULL))); else seedstr=(seed->text().simplified());
            //G->player()->name = name->text();
            //G->load_challenge(G->);//Game::SNOWMAN/*TWIN_HEAD*/);//设置冒险在这里改
            MW->load_page(NUMCOMBPREPPAGE);
        });


}
