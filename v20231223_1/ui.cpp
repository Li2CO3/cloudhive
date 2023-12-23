#include "ui.h"

void PvePage::Make_Summary()
{
    QDialog* dialog = new QDialog(MW->centralWidget());
    dialog->setWindowTitle("游戏记录");
    MW->G->monster->Make_Summary(dialog);

}

void Game::Monster::Make_Summary(QDialog* dialog)
{
    MainWindow* MW = G->MW;

    dialog->setFixedSize(600, 600);
    QString resulttext = "";
    resulttext = resulttext + "玩家: " + G->player->name + '\n';
    resulttext = resulttext + "挑战怪物: " + G->monster->name + "(血量" + QN(G->monster->initialhealth) + ")\n";
    resulttext = resulttext + G->monster->name + "剩余血量: " + QN(G->monster->health);
    if (G->monster->armor > 0)
        resulttext = resulttext + "<" + QN(G->monster->armor) + ">";
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
        QString str = "";
        if (i <= G->turn)
        {
            if (G->record.npieces[i] == 1)
                str = G->record.pieces[i][0].to_string();
            else
                str = G->record.pieces[i][G->record.cache[i].toInt()].to_string();

            QPixmap p = QPixmap("res/piece/card_" + str + ".png");
            piece->setPixmap(p.SCALED(32, 32));
            piece->setGeometry(80 + ((i - 1) % 10 * 45), 370 + (i - 1) / 10 * 45, 32, 32);
            piece->show();
        }
        pieces = pieces + str + " ";
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
    /*TODO...*/
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
    overall_out<<G->player->name<<"\t"<<G->monster->shortname<<"\t"<<G->monster->initialhealth<<"\t"<<G->turn<<"\t"<<G->monster->health<<"\t";
    overall_out<<G->player->totaldamage<<"\t"<<G->player->health<<"\t"<<G->player->point()<<"\t"<<G->monster->point<<"\t"<<G->random.getseed()<<"\t"<<i<<"\n";
    overall_record.close();
    /*TODO...*/
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
