#include "pvepage.h"
#include "monster.h"
#include "mainwindow.h"


void PvePage::load()
{
    {
        using namespace GT;
        gridimage = QPixmap();
        G->player->grid.Make_image(gridimage);
        NEW_LABEL_IMAGE_MW(grid_background, 150, 100, 896, 896, gridimage);

        NEW_BUTTON_IMAGE_MW(monster_image, 1450, 100, 100, 100, G->monster->icon);
        NEW_LABEL_IMAGE_MW(hero_image, 1100, 100, 100, 100, "res/icon/" + G->player->icon);

        //QString const Unknown_stats = QString("? ? ?/? ? ?").replace(" ","");
        QString const Player_Initial = QString("0/150");
        QString const Dragon_Initial = QString("0/2000");
        NEW_LABEL_ALIGN_MW(monster_name, 1250, 300, 300, 50, G->monster->name, 40, Right);
        NEW_LABEL_ALIGN_MW(hero_name, 1100, 300, 300, 50, G->player->name, 40, Left);
        NEW_LABEL_ALIGN_MW(monster_stats, 1250, 350, 300, 50, Dragon_Initial, 30, Right);
        NEW_LABEL_ALIGN_MW(hero_stats, 1100, 350, 300, 50, Player_Initial, 30, Left);
        //NEW_LABEL_ALIGN_MW(monster_stats2,350,50,300,50,G->monster->name+": "+Dragon_Initial,12,Left);
        //NEW_LABEL_ALIGN_MW(hero_stats2,350,70,300,50,G->player->name+": "+Player_Initial,12,Left);
        NEW_LABEL_ALIGN_MW(monster_extra, 1100, 400, 450, 580, "", 30, HCenter);
        //NEW_LABEL_ALIGN_MW(turninfo2,250,50,200,50,"第0回合",20,Left);
        NEW_LABEL_ALIGN_MW(turninfo, 1200, 250, 300, 50, "第1/35回合", 40, HCenter);
        NEW_BUTTON_MW(to_home, 20, 20, 60, 60, "退出", 20);

        MainWindow::connect(to_home, &QPushButton::clicked, this, [=]() {G->remake(); MW->load_page(STARTPAGE); });//强制退出

        for (int i = 1; i < 20; i++)
        {
            NEW_BUTTON_MW(keys[i], 150 + GRID_X[i] * 128, 36 + GRID_Y[i] * 64, 128, 128, "", 15);//QString::number(i));//放块
            keys[i]->setFlat(true);
        }
        NEW_BUTTON_IMAGE_MW(keys[0], 150 + GRID_X[0] * 128 + 4, 36 + GRID_Y[0] * 64 + 4, 120, 120, "res/icon/diu312.png");//0号
        keys[0]->setIconSize(QSize(rel(120), rel(120)));

        for (int i = 0; i < 20; i++)
            MainWindow::connect(
                keys[i], &QPushButton::clicked, this, [=]() {
                    monster_extra->setText("");
                    if (G->Check_Operation(0, cache_op, QN(i)))
                        emit G->signal_new_operation(cache_op + "," + QN(i));//放块效果
                });
        //给按钮连上放块
        connect(monster_image, &QPushButton::clicked, this, [=]() {Show_info_in_new_dialog(); });

        connect(G, SIGNAL(Alert_monster(QString)), this, SLOT(Update_Battle_Log(QString)));//更新指令
        connect(G, &Game::signal_update_turn_piece, this, &PvePage::Update_Turn_Piece);
        connect(G, &Game::signal_before_turn, this, [=]() {Update_Turns(); });
        connect(G, &Game::signal_player_change_stats, this, &PvePage::Update_Player_Stats);
        connect(G, &Game::signal_monster_change_stats, this, &PvePage::Update_Monster_Stats);
        connect(G, &Game::signal_game_end, this, [=]() {Make_Summary(); });

        //    G->monster->id=Game::TWIN_HEAD;
    }
}

void PvePage::Make_Summary()
{
    QDialog* dialog = new QDialog(MW->centralWidget());
    dialog->setWindowTitle("游戏记录");
    MW->G->monster->Make_Summary(dialog);

}

void Monster::Make_Summary(QDialog* dialog)
{
    MainWindow* MW = G->MW;

    dialog->setFixedSize(600, 600);
    QString resulttext = "";
    resulttext = resulttext + "玩家: " + G->player->name + '\n';
    resulttext = resulttext + "挑战怪物: " + G->monster->name + "(血量" + QN(G->monster->initialhealth) + ")\n";
    resulttext = resulttext + G->monster->name + "剩余血量: " + G->monster->stat_string().split('/')[1];
    resulttext = resulttext + '\n' + "累计造成伤害: " + QN(G->player->totaldamage) + '\n';
    resulttext = resulttext + "玩家终局血量: " + QN(G->player->health) + '\n';
    resulttext = resulttext + "终局得分: " + QN(G->player->point()) + '\n';
    resulttext = resulttext + G->monster->name + "终局得分: " + QN(G->monster->point) + '\n';
    resulttext = resulttext + "本局种子: " + QString::number(G->random.getseed());

    QFont f; f.setFamily("consolas"); f.setP(16);

    QLabel* finalgridinfo = new QLabel(dialog);
    SetLabel(finalgridinfo, 300, 10, 200, 20, 15);
    finalgridinfo->setText("终局盘面:");
    finalgridinfo->setFont(f);
    finalgridinfo->show();

    QLabel *versioninfo = new QLabel(dialog);
    SetLabel(versioninfo, 400, 10, 180, 20, 15);//版本
    versioninfo->setText("版本:"+__VERSION);
    versioninfo->setFont(f);
    versioninfo->setAlignment(Qt::AlignRight);
    versioninfo->show();

    QLabel* finalgrid = new QLabel(dialog);
    QPixmap p;
    G->player->grid.Make_image(p);
    finalgrid->setPixmap(p.SCALED(224, 224));
    finalgrid->setGeometry(300, 30, 224, 224);
    finalgrid->show();

    QLabel* operations = new QLabel("", dialog);
    SetLabel(operations, 10, 5, 400, 400, 15); operations->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    operations->setFont(f);
    resulttext += "\n本局操作:\n";
    for (int i = 1; i <= MAX_TURN; i++)
    {
        if (i <= G->turn)
        {
            if (G->record[i].toInt() < 10)resulttext += " ";
            resulttext += G->record[i] + ' ';
        }
        if (i % 10 == 0)resulttext += '\n';
    }

    resulttext += "\n本局发牌:\n";

    QString pieces = "";
    for (int i = 1; i <= MAX_TURN; i++)
    {
        QLabel* piece = new QLabel(dialog);
        Piece p;
        if (i <= G->turn)
        {
            if (G->record.npieces[i] == 1)
                p = G->record.pieces[i][0];
            else
                p = G->record.pieces[i][G->record.cache[i].toInt()];

            QPixmap map = QPixmap(p.image());
            piece->setPixmap(map.SCALED(32, 32));
            piece->setGeometry(80 + ((i - 1) % 10 * 45), 370 + (i - 1) / 10 * 45, 32, 32);
            piece->show();

            pieces = pieces + p.to_string() + " ";
        }
        if (i % 10 == 0)pieces += "\n";
    }

    resulttext += pieces;
    operations->setText(resulttext); operations->show();

    operations->setTextInteractionFlags(Qt::TextSelectableByMouse);

    dialog->show();
    QPushButton* view_details = new QPushButton(dialog);
    view_details->setGeometry(180, 550, 100, 40);
    view_details->setText("查看详情");
    //view_details->show();
    //view_details->connect(view_details,&QPushButton::clicked, MW,[=](){Make_Detailed_Recap();});
    QPushButton* restart = new QPushButton(dialog);
    restart->setGeometry(180, 550, 100, 40);
    restart->setText("再来一局");
    restart->show();
    restart->connect(restart, &QPushButton::clicked, MW, [=]() {MW->G->remake(); MW->load_page(PVEPREP); });
    QPushButton* backtogame = new QPushButton(dialog);
    backtogame->setGeometry(320, 550, 100, 40);
    backtogame->setText("返回游戏");
    backtogame->show();
    backtogame->connect(backtogame, &QPushButton::clicked, MW, [=]() {dialog->close(); });
    QPushButton* back = new QPushButton(dialog);
    back->setGeometry(460, 550, 100, 40);
    back->setText("返回主页");
    back->show();
    back->connect(back, &QPushButton::clicked, MW, [=]() {G->remake(); MW->load_page(STARTPAGE); });

    operations->setTextInteractionFlags(Qt::TextSelectableByMouse);
    /*TODO...修改创建记录文件的代码位置*/
    {//创建记录文件所在文件夹
    QDir dir =QDir("res/record/records");
    if(!dir.exists())
        if(!dir.mkpath(".")){/*Alert("记录文件创建失败!");*/}
    }
    QString str;
    for(int i=1;;i++)
    {
        str="res/record/records/"+(G->monster->name)+"_"+QN(G->random.getseed())+"_"+QN(i)+".txt";
        if(!QFile::exists(str))break;
    }
    QFile record_file(str);

    record_file.open(QIODevice::WriteOnly | QIODevice::Text) ;
    QTextStream out(&record_file);
    out<<resulttext;
    record_file.close();

    QFile overall_record("res/record/overall_records.txt");

    int i=str.split(".")[0].split("_")[2].toInt();

    overall_record.open(QIODevice::Append | QIODevice::Text) ;
    QTextStream overall_out(&overall_record);
    overall_out<<G->player->name<<"\t"<<G->monster->shortname<<"\t"<<G->monster->initialhealth<<"\t"<<G->turn<<"\t";
    {QString str=G->monster->stat_string().split('/')[1];while(str.length()<8) {str = str + ' ';} overall_out<<str;}
    overall_out<<"\t";
    overall_out<<G->player->totaldamage<<"\t";
    overall_out<<G->player->health<<"\t";
    overall_out<<G->player->point()<<"\t";
    overall_out<<G->monster->point<<"\t";
        for(int i=QN(G->random.getseed()).length();i<10;i++)overall_out<<' ';
        overall_out<<QN(G->random.getseed());
    overall_out<<"\t"<<i<<"\t"<<__VERSION<<"\n";
    overall_record.close();

    /*TODO...移动写文件的地方*/
}


void PvePage::Show_info_in_new_dialog()
{
    QDialog* dialog = new QDialog(MW->centralWidget());
    //dialog->setWindowModality(Qt::ApplicationModal);
    //dialog->setModal(false);
    QLabel* icon;
    QLabel* desc;

    dialog->setWindowIcon(QPixmap(G->monster->icon));
    dialog->setWindowTitle(G->monster->name);
    NEW_LABEL_IMAGE(icon, 450, 15, 90, 90, G->monster->icon, dialog);
    QString str = G->monster->description();
    NEW_LABEL_ALIGN(desc, 12, 120, 990, 560, str, 22, Top | Qt::AlignLeft, dialog);
    desc->setTextInteractionFlags(Qt::TextSelectableByMouse);

    desc->setFixedWidth(990);
    icon->setPixmap(QPixmap(G->monster->icon).scaled(icon->size()));
    desc->adjustSize();
    desc->setGeometry(0, 120, desc->width(), desc->height());
    dialog->setFixedSize(990, desc->height() + 130);
    dialog->show();
    QPushButton* close = new QPushButton("确定", dialog); close->setGeometry(dialog->width() - 110, dialog->height() - 60, 80, 30); close->show();
    connect(close, &QPushButton::clicked, this, [=]() {dialog->close(); dialog->reject(); });
    connect(dialog, &QDialog::rejected, this, [=]() {dialog->deleteLater(); });
}
