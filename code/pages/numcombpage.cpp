#include "NumcombPage.h"
#include "monster.h"
#include "mainwindow.h"
#include "utils/comb_assembler.hpp"

void NumcombPage::load()
{
    {
        using namespace GT;
        for(int i=0;i<nplayer;i++)
        {
            gridimage[i] = QPixmap();
            G->player(i)->grid.Make_image(gridimage[i],GT::NUM_GRID);

        NEW_LABEL_IMAGE_MW(grid_background[i], 150+SHIFT_X, 100+SHIFT_Y, 896, 896, gridimage[i]);
        NEW_LABEL_IMAGE_MW(hero_image[i], 1230+i*50, 200, 70, 70, "res/icon/" + G->player(i)->icon);

        //QString const Unknown_stats = QString("? ? ?/? ? ?").replace(" ","");
        QString const Player_Initial = QString("当前得分: 0");
        NEW_LABEL_ALIGN_MW(hero_name[i], 1340+i*50, 210, 300, 50, G->player(i)->name, 40, Left);
        NEW_LABEL_ALIGN_MW(hero_point[i], 1200+i*50, 370, 300, 50, Player_Initial, 30, HCenter);
        }
        NEW_LABEL_ALIGN_MW(turninfo, 1200, 320, 300, 50, "第1/20回合", 40, HCenter);
        NEW_BUTTON_MW(to_home, 20, 20, 60, 60, "退出", 20);

        MainWindow::connect(to_home, &QPushButton::clicked, this, [=]() {G->remake(); MW->load_page(STARTPAGE); });//强制退出

        for (int i = 0; i < 20; i++)
        {
            NEW_BUTTON_MW(keys[i], 150 + SHIFT_X + GRID_X[i] * 128, 36 + SHIFT_Y + GRID_Y[i] * 64, 128, 128, "", 15);//QString::number(i));//放块
            keys[i]->setFlat(true);
        }

        for (int i = 0; i < 20; i++)
            MainWindow::connect(
                keys[i], &QPushButton::clicked, this, [=]() {
                    if (G->Check_Operation(0, cache_op, QN(i)))
                        emit G->signal_new_operation(cache_op + "," + QN(i));//放块效果
                });

        //connect(G, SIGNAL(Alert_monster(QString)), this, SLOT(Update_Battle_Log(QString)));//更新指令
        connect(G, &Game::signal_update_turn_piece, this, &NumcombPage::Update_Turn_Piece);
        connect(G, &Game::signal_before_turn, this, [=]() {Update_Turns(); });
        connect(G, &Game::signal_player_change_stats, this, &NumcombPage::Update_Player_Stats);
        connect(G, &Game::signal_game_end, this, [=]() {Make_Summary(); });

        //    G->monster()->id=Game::TWIN_HEAD;
    }
}

void NumcombPage::Make_Summary()
{
    QDialog* dialog = new QDialog(MW->centralWidget());
    dialog->setWindowTitle("游戏记录");
    //MW->G->monster()->Make_Summary(dialog);

    MainWindow* MW = G->MW;

    dialog->setFixedSize(600, 600);
    QString resulttext = "\n\n";
    resulttext = resulttext + "玩家: " + G->player()->name + '\n';
    resulttext = resulttext + "终局得分: " + QN(G->player()->point()) + '\n';
                 resulttext = resulttext + "本局种子: " +(G->random->getseed());
    resulttext+= "\n\n\n";
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
    G->player()->grid.Make_image(p,GT::NUM_GRID);
    finalgrid->setPixmap(p.SCALED(224, 224));
    finalgrid->setGeometry(300, 30, 224, 224);
    finalgrid->show();

    QLabel* operations = new QLabel("", dialog);
    SetLabel(operations, 10, 5, 400, 400, 15); operations->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    operations->setFont(f);
    resulttext += "\n本局操作:\n";
    for (int i = 1; i <= PVE_MAX_TURN; i++)
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
    for (int i = 1; i <= PVE_MAX_TURN; i++)
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
    restart->connect(restart, &QPushButton::clicked, MW, [=]() {MW->G->remake(); MW->load_page(NUMCOMBPREPPAGE); });
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

    /////拼装毁灭者！！！
    QPushButton* calc = new QPushButton(dialog);
    calc->setGeometry(40, 550, 100, 40);
    calc->setText("拼装毁灭");
    calc->show();
    calc->connect(calc, &QPushButton::clicked, MW, [=]()
                  {
        QDialog * dialog = new QDialog(MW);
        Comb_Assembler::Make_Assemble_Page("seed "+G->random->getseed(), dialog);
    });

    /// 拼装毁灭者！！！





    operations->setTextInteractionFlags(Qt::TextSelectableByMouse);
    /*TODO...修改创建记录文件的代码位置*/
    {//创建记录文件所在文件夹
    QDir dir =QDir("res/record/records/standard");
    if(!dir.exists())
        if(!dir.mkpath(".")){/*Alert("记录文件创建失败!");*/}
    }
    QString str;
    int trialtimes=1;
    for(;;trialtimes++)
    {
        str="res/record/records/standard/"+(G->random->getseed())+"_"+QN(trialtimes)+".txt";
        if(!QFile::exists(str))break;
    }
    QFile record_file(str);

    record_file.open(QIODevice::WriteOnly | QIODevice::Text) ;
    QTextStream out(&record_file);
    out<<resulttext;
    record_file.close();

    QFile overall_record("res/record/overall_records.txt");


    overall_record.open(QIODevice::Append | QIODevice::Text) ;
    QTextStream overall_out(&overall_record);
    overall_out<<G->player()->name<<"\t";
    overall_out<<"数字巢"<<"\t";
    overall_out<<G->player()->point()<<"\t";
        for(int i=(G->random->getseed()).length();i<10;i++)overall_out<<' ';
        overall_out<<(G->random->getseed());
    overall_out<<"\t"<<trialtimes<<"\t"<<__VERSION<<"\n";
    overall_record.close();

}
