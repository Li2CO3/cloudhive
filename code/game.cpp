#include "mainwindow.h"
#include "ui.h"
#include "game.h"

#include "monster.h"
#include "cardpool.h"
#include "modes/pve.h"
#include "modes/numcomb.h"
#include "utils/random.hpp"
#include <QMessageBox>
#include <QDir>

Game::Game() {
    connect(this, SIGNAL(signal_new_operation(QString)), this, SLOT(recv_operation(QString)));

    Clear();
    status = NOT_STARTED;
    gametype=GT::TYPE_NULL;
    //pool = new CardPool(&this->random);
    //player = new Player(this);
    //monster = new Monster(this);
}
Game::~Game() {
    //qDebug()<<"REMAKED";
    //if (pool)delete pool; // if (player())delete player(); if (monster())delete monster();//Debug()<<"DELETE GAME";
}


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
    MainWindow mw;
    MainWindow*w = &mw;
    Game* g = new PveGame();
    w->G = g;
    g->MW = w;
    w->resize(1200, 750);
    w->show();

	{


        QDir dir =QDir("res");
        if(!dir.exists())
        {int t=QMessageBox::warning(w,"资源包错误","未找到资源包(版本:"+__RES_VERSION+")","继续","退出","",0,0);//改文字太麻烦了！先deprecated着吧
            if(t==1)return 0;
        }


        QString ver = "res/game/version.txt";
        QFile fver(ver);

        QString content;
        if (fver.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream inStream(&fver);
            content = inStream.readAll();

            if(content==__RES_VERSION)
            ;
            else
            {int t=QMessageBox::warning(w,"资源包错误","错误的版本(需要版本:"+__RES_VERSION+","+"当前版本:"+content+")","继续","退出","",0,0);//改文字太麻烦了！先deprecated着吧
            if(t==1)return 0;
            }

        }
        else
        {int t=QMessageBox::warning(w,"资源包错误","未知的版本(需要版本:"+__RES_VERSION+")","继续","退出","",0,0);//改文字太麻烦了！先deprecated着吧
            if(t==1)return 0;
        }


        QFontDatabase::addApplicationFont("res/game/OPPOSans-R.ttf");
		QString K = "res/game/default_font.txt";
        QFile file(K);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream inStream(&file);
			content = inStream.readAll();
            content = content.split("\n")[0];
            QApplication::setFont(QFont(content));
		}
        file.close();
        QFont f;
        f.setPixelSize(18);
        QApplication::setFont(f);
    }

	w->load_page(STARTPAGE);
	return a.exec();
	//w->G->RUN();
}
Game * GameMaker::newGame(GT::GAMETYPE type)
{
    switch(type)
    {
    case GT::PVEGAME:
        return new PveGame();
    case GT::NUMCOMBGAME:
        return new NumcombGame();
    default:
        return new Game();
        //throw 0;
    }


}
void Game::remake(GT::GAMETYPE type)
{
    MW->G = GameMaker::newGame(type);
    MW->G->MW = MW;
    MW->G->player()->name=MW->name;
    MW->G->player()->icon=MW->icon;
//    MW->G->scoring_rule=scoring_rule;
    ///todo:此处信息的复制并不完整
    //保留：
    //MW->G->player()->name = player()->name;
    //MW->G->player()->icon = player()->icon;
    assert(type!=GT::TYPE_NULL);

        MW->G->setrandom(random->getseed());


    this->disconnect();
    //	this->terminate();
    deleteLater();
}
void Game::setrandom(QString seed)
{
    bool isnumber;
    int t=seed.toInt(&isnumber);
    Q_UNUSED(t);
    GT::RANDOMTYPE type;
    if(isnumber)
    {type=GT::SIMPLERANDOM;}
    else
    {type=GT::MT19937RANDOM;}

    RandomMaker::make_random(random,type,seed);
}

QString GT::toFileName(QString str)
{
    QString result = "";
    QChar check[10]={'\\','/','|','<','>','*',':','?','\"','`'};
    QString out[10]={"`反","`斜","`竖","`小","`大","`星","`冒","`问","`引","``"} ;
    for(int i=0;i<str.length();i++)
    {
        QChar c=str[i];
        // \/|<>*:?"

        int k=0;
        for(;k<10;k++)
            if(c==check[k])
            {result.append(out[k]);
                break;}
        if(k==10)
            result.append(c);
    }
    if(result.length()>50) return result.left(20)+"...[文件名过长]";
    return result;


}
