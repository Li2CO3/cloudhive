#include "mainwindow.h"
#include "ui.h"
#include "game.h"

#include "monster.h"
#include "cardpool.h"
#include "modes/pve.h"

Game::Game() {
    connect(this, SIGNAL(signal_new_operation(QString)), this, SLOT(recv_operation(QString)));

    Clear();
    connect(this, &Game::signal_before_turn, this, [=]() {Before_Turn(); });
    connect(this, &Game::signal_turn, this, [=]() {Locate_Piece(); });
    connect(this, &Game::signal_before_combat, this, [=]() {Before_Combat(); });
    connect(this, &Game::signal_combat, this, [=]() {Combat(); });
    connect(this, &Game::signal_after_combat, this, [=]() {After_Combat(); });
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
	{
        QFontDatabase::addApplicationFont("res/game/OPPOSans-R.ttf");
		QString K = "res/game/default_font.txt";
		QFile file(K);
		QString content;

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
    MainWindow mw;
    MainWindow*w = &mw;
    Game* g = new Game();
	w->G = g;
	g->MW = w;
	w->resize(1200, 750);
	w->show();
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
    ///todo:此处信息的复制并不完整
    //保留：
    MW->G->random.setseed(random.getseed());
    //MW->G->player()->name = player()->name;
    //MW->G->player()->icon = player()->icon;
    if(gametype!=GT::TYPE_NULL)
    {
        MW->G->monster() = Monster::new_monster(monster()->id, MW->G);
    }
    this->disconnect();
    //	this->terminate();
    deleteLater();
}
