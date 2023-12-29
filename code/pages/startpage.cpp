#include "startpage.h"

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
                    G->player->set_icon(Icons[ICONS_PER_PAGE * (bottom_label->text().toInt() - 1) + id]);
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
        connect(prev,&QPushButton::clicked,this,[=](){

            int newpage=bottom_label->text().toInt()-1;
            bottom_label->setText(QN(newpage));
            if(newpage>1) prev->show(); else prev->hide();
            if(newpage<(Icons.length()-1)/ICONS_PER_PAGE+1) next->show();else next->hide();
        });
        if(page>=1)prev->show();else prev->hide();

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
        if(page<(Icons.length()-1)/ICONS_PER_PAGE+1)next->show();else next->hide();


    }

}
void StartPage::load()
    {
        G->player->load_name();
        NEW_LABEL_ALIGN_MW(versioninfo,1400,20,180,80,"版本:"+__VERSION,20,Top |Qt::AlignRight);
        NEW_BUTTON_IMAGE_MW(myicon, 665, 200, 70, 70, "res/icon/" + G->player->icon);
        NEW_BUTTON_MW(myname, 765, 200, 170, 70, G->player->name, 30);

        NEW_BUTTON_MW(newgame, 650, 300, 300, 150, "开始游戏", 30);
        connect(newgame, &QPushButton::clicked, MW, [=]() {MW->load_page(PVEPREP); });//qDebug()<<"xxxx\n";/*new_game()*/;});
        NEW_BUTTON_MW(quitgame, 650, 700, 300, 70, "退出游戏", 30);
        connect(quitgame, &QPushButton::clicked, MW, [=]() {qApp->quit();/*quit_game()*/; });

        connect(myname, &QPushButton::clicked, this, [=]() {
            QDialog* dialog = new QDialog(MW->centralWidget());//设置名字
            dialog->setWindowTitle("设置昵称");
            dialog->setModal(true);
            dialog->setFixedSize(160, 120); dialog->show();
            QFont f;
            /*f.setFamily("OPPOSans R");*/f.setP(15);
            QLabel* lab = new QLabel("你的名字:", dialog);
            SetLabel(lab, 5, 5, 100, 30, 16);
            QLineEdit* rename = new QLineEdit(G->player->name, dialog);
            rename->setFont(f);
            rename->setGeometry(5, 35, 110, 40);
            lab->show(); rename->show();

            QPushButton* sure = new QPushButton("确定", dialog);
            QPushButton* cancel = new QPushButton("取消", dialog);
            sure->setFont(f); cancel->setFont(f);
            sure->setGeometry(20, 80, 50, 30); cancel->setGeometry(90, 80, 50, 30);
            sure->show(); cancel->show();
            connect(sure, &QPushButton::clicked, this, [=]() {G->player->set_name(rename->text()); dialog->close(); MW->load_page(STARTPAGE); });
            connect(cancel, &QPushButton::clicked, this, [=]() {dialog->close(); dialog->deleteLater(); });
            });
        connect(myicon, &QPushButton::clicked, this, [=]() {show_icons(1); });

        NEW_BUTTON_MW(settings, 650, 500, 300, 70, "窗口设置", 30);
        connect(settings, &QPushButton::clicked, MW, [=]() {MW->load_page(SETTINGS);/*quit_game()*/; });
    }
