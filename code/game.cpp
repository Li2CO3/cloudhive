#include "mainwindow.h"
#include "ui.h"
#include "game.h"

#include "monster.h"
#include "cardpool.h"


Game::Game() {
    connect(this, SIGNAL(signal_new_operation(QString)), this, SLOT(recv_operation(QString)));

    Clear();
    connect(this, &Game::signal_before_turn, this, [=]() {Before_Turn(); });
    connect(this, &Game::signal_turn, this, [=]() {Locate_Piece(); });
    connect(this, &Game::signal_before_combat, this, [=]() {Before_Combat(); });
    connect(this, &Game::signal_combat, this, [=]() {Combat(); });
    connect(this, &Game::signal_after_combat, this, [=]() {After_Combat(); });
    status = NOT_STARTED;
    mode = MODE_NULL;
    pool = new CardPool(&this->random);
    player = new Player(this);
    monster = new Monster(this);
}
Game::~Game() {
    //qDebug()<<"REMAKED";
    if (pool)delete pool;  if (player)delete player; if (monster)delete monster;//Debug()<<"DELETE GAME";
}

void Game::Player::load_name()
{
    QFile file("res/game/user.txt");
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream inStream(&file);
        content = inStream.readAll();

        G->player->name = content.split("\n")[0];
        G->player->icon = content.split("\n")[1];
        file.close();
    }
}

void Game::Player::set_name(QString newname)
{
    name = newname;

    QString filename = "res/game/user.txt";
    QString str = name + "\n" + icon;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << str;
    file.close();
}

void Game::Player::set_icon(QString newicon)
{
    icon = newicon;

    QString filename = "res/game/user.txt";
    QString str = name + "\n" + icon;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << str;
    file.close();
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

void Game::Start(GAMEMODE mod)
{
    mode=mod;
	switch (mode)
	{
	case MODE_PVE:
	{
		status = BUSY;
		monster->reset();
		player->reset();
	}
	break;
	default:throw 0;
	}

	Before_Turn();
}

void Game::Before_Turn()
{
	player->prev_point = player->point();
	if (turn == MAX_TURN) { Game_End(); return; }
	turn++;

	switch (mode)
	{
	case MODE_PVE:
		monster->Monster_Before_Turn();

        break;
    default:throw 0;break;
	}

	//qDebug()<<"status="<<status;
	if (status == BUSY)
        emit signal_turn();
}

void Game::Locate_Piece()
{
	//qDebug()<<"locate";
	//switch mode
	monster->setStatus();
	emit signal_wait_for_operation();
	return;
}

void Game::Before_Combat()
{
	switch (mode)
	{
	case MODE_PVE:
	{
		monster->Monster_Before_Combat();
	}
    break;
    default:throw 0;break;
	}

	//qDebug()<<"status="<<status;
	if (status == BUSY)
        emit signal_combat();
}

void Game::Combat()
{
	//qDebug()<<"combat";
	switch (mode)
	{
	case MODE_PVE:
	{
		monster->Monster_Combat();
    }
    break;
    default:throw 0;break;
	}
	//qDebug()<<"status="<<status;
	if (status == BUSY)
        emit signal_after_combat();
}

void Game::After_Combat()
{
	//qDebug()<<"aftercombat";

	//qDebug()<<"status="<<status;
	switch (mode)
	{
	case MODE_PVE:
	{
		monster->Monster_After_Combat();
    }
        break;
    default:throw 0;
        break;
	}
	if (status == BUSY)
        emit signal_before_turn();
}

void Game::remake()
{
	MW->G = new Game();
	MW->G->MW = MW;

	//保留：
	MW->G->random.setseed(random.getseed());
	MW->G->player->name = player->name;
	MW->G->player->icon = player->icon;
	MW->G->monster = Monster::new_monster(monster->id, MW->G);
	this->disconnect();
//	this->terminate();
	deleteLater();
}

void Game::recv_operation(QString expr)
{
	if (expr == "QUIT")
	{
        //quit();
        remake(); return;
	}

	if (status >= 0)return;//NOT WAITING
	if (expr.count(",") > 1)throw 0;
	QString cache = expr.split(",")[0];
	QString op = expr.split(",")[1];
	switch (mode)
	{
	case MODE_PVE:
	{
		Player* ply = player;
		Piece p;
		if (status == WAIT_TURN)p = pool->current[0];
		else if (status == WAIT_CHOOSE_TURN)p = pool->current[cache.toInt()];
        else if (status == WAIT_SNOWMAN_DISCARD) p = Piece(0, 0, 0);
		int place = op.toInt();

		record.covered[turn] = ply->grid.pieces[place];
		ply->Locate_piece(p, place);
		status = BUSY;
		record.cache[turn] = cache;
		record[turn] = op;
        emit signal_before_combat();
		break;
    }
    default:throw 0;break;
	}
}

bool Game::Check_Operation(int ply, QString cache, QString op)
{
	Q_UNUSED(ply)
		//switch(mode)
	{
		//default:
		switch (status)
		{
		case WAIT_TURN:
			return true;
			break;
		case WAIT_CHOOSE_TURN:
			return cache != "";
			break;
        case WAIT_SNOWMAN_DISCARD:
            for (int place = 1; place <= 19; place++)if (! (player->grid.pieces[place].isEmptyPiece()) )//
                      return (op.toInt() != 0) && (! (player->grid.pieces[op.toInt()] .isEmptyPiece()) );
			return true;
		default:
			return false;
		}
	}

	return true;
}

void Game::Player::Locate_piece(Piece p, int place)
{
	int old_pt = point();
	Piece before = *grid[place];
	*grid[place] = p;
	G->MW->Set_Piece(p, place);//player info
	int new_pt = point();
	bool changed_pt = (new_pt != old_pt);
    if (changed_pt) { emit changePoint(); }

	//后续的boss的反应都在这里
}

void Game::sync_record() {//TODO
    record.npieces[turn] = pool->ncurrent;
    for (int i = 0; i < pool->ncurrent; i++)record.pieces[turn][i] = pool->current[i];
}


void Game::load_challenge(GT::MONSTER_ID monst)
{
    //pool=new Pool(this);
    //player=new Player(this);
    //现在pool和player没什么特殊的。如果需要可以参照monster

    Monster::new_monster(monst, this);
    scoring_rule = monster->scoringrule();
    pool->setpool(monster->pooltype());
    player->health = 150;
    return;
}
