﻿#include "mainwindow.h"
#include "ui.h"
#include "game.h"

void Game::Grid::Make_image(QPixmap& img)
{
	img = QPixmap(64 * 7, 64 * 7);
	QPainter painter(&img);

	QPixmap bordercolor = QPixmap(1, 1);
	bordercolor.fill(QColor::fromRgb(52, 90, 136));
	for (int c = 0; c < 7; c++)
	{
		if (c != 0)
			painter.drawPixmap(-2 + 64 * c, 0, bordercolor.scaled(4, 448));
		for (int r = (c) % 2 + 1; r < 14; r += 2)
			painter.drawPixmap(c * 64, -2 + r * 32, bordercolor.scaled(64, 4));
	}
	//画边框
	//rgb(52,90,136);

	for (int place = 1; place <= 19; place++)
	{
        if (pieces[place] != 0)
			painter.drawPixmap(GRID_X[place] * 64, GRID_Y[place] * 32 - 32, QPixmap("res/piece/card_" + (*this)[place]->to_string() + ".png"));
		else
			painter.drawPixmap(GRID_X[place] * 64, GRID_Y[place] * 32 - 32, QPixmap("res/piece/num_" + QString::number(place) + ".png"));
	}
	auto find_xydir = [=](int x, int y, int dir) {
		int dir_val = -1;
		int dx, dy;
		if (dir == 3) { dx = 1, dy = 1; }
		else if (dir == -3) { dx = -1, dy = -1; }

		else if (dir == 1) { dx = 0; dy = 2; }
		else if (dir == -1) { dx = 0; dy = -2; }
		else if (dir == 2) { dx = 1; dy = -1; }
		else if (dir == -2) { dx = -1; dy = 1; }
		else throw 0;
		for (int k = 1;; k++)
		{
			int targetx = x + k * dx; int targety = y + k * dy;
			if (targetx > 6)break;
			if (targetx < 0)break;
			if (targety < 1)break;
			if (targety > 13)break;
			int targetid = place_ids[targety][targetx];
			if (targetid >= 20)
			{
				if (dir_val != -1) { break; }
				else { continue; }
			}
			int newval;
			if (dir == 3 || dir == -3)newval = (*this)[targetid]->x348();
			if (dir == 2 || dir == -2)newval = (*this)[targetid]->x267();
			if (dir == 1 || dir == -1)newval = (*this)[targetid]->x159();
			if (dir_val == -1 || dir_val == 10)
			{
				dir_val = newval; continue;
			}
			if (newval == 10)continue;
			if (dir_val != newval)return 0;
			continue;
		}
		return dir_val;
	};

	for (int place = 20; place <= 45; place++)
	{
		int x = GRID_X[place], y = GRID_Y[place];
		QString wall_desc = "000";
		if (find_xydir(x, y, 3) > 0 || find_xydir(x, y, -3) > 0) { wall_desc[0] = '1'; }
		if (find_xydir(x, y, 2) > 0 || find_xydir(x, y, -2) > 0) { wall_desc[2] = '1'; }
		if (find_xydir(x, y, 1) > 0 || find_xydir(x, y, -1) > 0) { wall_desc[1] = '1'; }
		//find_xydir(0,2,3);find_xydir(0,2,-3);
		painter.drawPixmap(x * 64, y * 32 - 32, QPixmap("res/piece/wall_" + wall_desc + ".png"));
	}

	for (int x = 0; x < 7; x += 2)
	{
		painter.drawPixmap(x * 64, 0, QPixmap("res/piece/wall_half.png"));
		painter.drawPixmap(x * 64, 32 * 13, QPixmap("res/piece/wall_half.png"));
	}
	//0~6 0~14 center=3,7

	//1,2 2,1 3,2 4,1 5,2
	//    2,3     4,3
	//....
	//1,14 2,13 3,14 4,13 5,14
	//  2,15  4,15
}

int Game::Grid::point(SCORING_RULE rule)
{
	/*
		auto effectof = [=](int place, int dir){
			if(place>19)return 0;
			int x=GRID_X[place],y=GRID_Y[place];
			int dx,dy;
			if(dir==3){dx=1;dy=1;}
			else if(dir==2){dx=1;dy=-1;}
			else if(dir==1){dx=0;dy=2;}
			else throw 0;

			int cur_val=pieces[place].ValueofLine(dir);
			//int count_cells=1;
			for(int k=-1;k<=1;k+=2)
				for(int s=1;;s++)
				{int step=k*s;

				int x2=x+step*dx,y2=y+step*dy;
				if(x2>6||x2<0||y2>13||y2<1)break;
				int newid=place_ids[y2][x2];
				if(newid>19)break;
				//count_cells++;
				int newvalue=pieces[newid].ValueofLine(dir);
				if(cur_val==10){cur_val=newvalue;continue;}
				if(newvalue==10)continue;
				if(cur_val==newvalue)continue;
					return 0;
			}
			return cur_val;
		};
		int pt=0;
		for(int i=1;i<=19;i++)
			for(int dir=1;dir<=3;dir++)
				pt+=effectof(i,dir);
		return pt;
	*/
	//int a[2][1]={{1},{2}};
	auto f = [=](int a, int b) {if (a == 10)return b; if (b == 10)return a; if (a == b)return a; return 0; };

	int l267[5][5] = { {1,4,8,-1,-1},{2,5,9,13,-1},{3,6,10,14,17},{7,11,15,18,-1},{12,16,19,-1,-1} };
	int l159[5][5] = { {1,2,3,-1,-1},{4,5,6,7,-1},{8,9,10,11,12},{13,14,15,16,-1},{17,18,19,-1,-1} };
	int l348[5][5] = { {8,13,17,-1,-1},{4,9,14,18,-1},{1,5,10,15,19},{2,6,11,16,-1},{3,7,12,-1,-1} };
	int r267[5] = { 10,10,10,10,10 }, r159[5] = { 10,10,10,10,10 }, r348[5] = { 10,10,10,10,10 };
	for (int l = 0; l < 5; l++)
		for (int p = 0; p < 5; p++)
		{
			int t = l267[l][p]; if (t > 0) r267[l] = f(r267[l], pieces[t].x267());
			t = l159[l][p]; if (t > 0) r159[l] = f(r159[l], pieces[t].x159());
			t = l348[l][p]; if (t > 0) r348[l] = f(r348[l], pieces[t].x348());
		}
	int basic_point = 0; int const coef[5] = { 3,4,5,4,3 };
	for (int i = 0; i < 5; i++) basic_point += (r267[i] + r159[i] + r348[i]) * coef[i];
	int points = basic_point;
	switch (rule)
	{
	case Scoring_Normal:
		return basic_point;
	case One_Lines_Plus_12:
		for (int i = 0; i < 5; i++) if (r159[i] == 1)points += 12; return points;
	case One_Lines_Plus_15:
		for (int i = 0; i < 5; i++) if (r159[i] == 1)points += 15; return points;
    default:
        return basic_point;
	}
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

	QString filename = QCoreApplication::applicationDirPath() + "/res/game/user.txt";
	QString str = name + "\n" + icon;
	QFile file(filename);
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out << str;
	file.close();
}

void Game::Pool::setpool(int type)
{
	int line348[10] = { 3,4,8 };
	int line159[10] = { 1,5,9 };
	int line267[10] = { 2,6,7 };
	int n348 = 3, n159 = 3, n267 = 3;
	int nANY = 2, nset = 2;
    npiece = 0;
	switch (type)
	{
	case 0://default
	default:
		break;
	case -1://no 1
		line159[0] = 5; line159[1] = 9; n159 = 2; nset = 3;
		break;
	case -2://no 2
		line267[0] = 5; line267[1] = 9; n267 = 2; nset = 3;
		break;
	case -3://no 3
		line348[0] = 4; line348[1] = 8; n348 = 2; nset = 3;
		break;
	case -4://no 4
		line348[0] = 3; line348[1] = 8; n348 = 2; nset = 3;
		break;
	case -5://no 5
		line159[0] = 1; line159[1] = 9; n159 = 2; nset = 3;
		break;
	case -6://no 6
		line267[0] = 2; line267[1] = 7; n267 = 2; nset = 3;
		break;
	case -7://no 7
		line267[0] = 2; line267[1] = 6; n267 = 2; nset = 3;
		break;
	case -8://no 8
		line348[0] = 3; line348[1] = 4; n348 = 2; nset = 3;
		break;
	case -9://no 9
		line159[0] = 1; line159[1] = 5; n159 = 2; nset = 3;
		break;
	case 1000://调色盘
		nANY = 5;
		break;
	case 66:
		line267[0] = 2; line267[1] = 2; line267[2] = 6; line267[3] = 6; line267[4] = 6; line267[5] = 7; line267[6] = 7;
		n267 = 7; nset = 1;
		break;
    case 10101:
        int num = 3;
        while (num--) {
            this->back(317);
            this->back(397);
            this->back(412);
            this->back(416);
            this->back(452);
            this->back(496);
            this->back(812);
            this->back(856);
            this->back(892);
            this->back(896);
        }
        break;

	}
    for (int a = 0; a < n348; a++) {
        for (int b = 0; b < n159; b++) {
            for (int c = 0; c < n267; c++) {
                for (int setid = 0; setid < nset; setid++)
                {
                    this->back(line348[a] * 100 + line159[b] * 10 + line267[c]);
                }
            }
        }
	}
	for (int anys = 0; anys < nANY; anys++)
	{
        this->back(1000);
	}
}

void MainWindow::load_page(PageType type)
{
	clear_all_object();

	switch (type)
	{
	case STARTPAGE:
	{new StartPage(this); }
	break;
	case PVEPAGE:
	{new PvePage(this); }
	break;
	case PVEPREP:
	{new PvePrep(this); }
	break;
	case SETTINGS:
	{new SettingsPage(this); }
	default:break;
	}
	page->load();
	//qDebug()<<"pagetype set to="<<type<<"===";
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
	}
	MainWindow* w = new MainWindow;
	Game* g = new Game();
	w->G = g;
	g->MW = w;
	w->resize(1200, 750);
	w->show();
	w->load_page(STARTPAGE);
	return a.exec();
	//w->G->RUN();
}

void Game::Start()
{
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
	default:break;
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
	default:break;
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
	default:break;
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
	default:break;
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
	this->terminate();
	deleteLater();
}

void Game::recv_operation(QString expr)
{
	if (expr == "QUIT")
	{
		quit(); remake(); return;
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
		else if (status == WAIT_SNOWMAN_DISCARD) p = 0;
		int place = op.toInt();

		record.covered[turn] = ply->grid.pieces[place];
		ply->Locate_piece(p, place);
		status = BUSY;
		record.cache[turn] = cache;
		record[turn] = op;
        emit signal_before_combat();
		break;
	}
	default:break;
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
			for (int place = 1; place <= 19; place++)if (player->grid.pieces[place] != 0)//
				return (op.toInt() != 0) && (player->grid.pieces[op.toInt()]) != Piece(0);
			return true;
		default:
			return false;
		}
	}

	return true;
}

void Game::Player::Locate_piece(Game::Piece p, int place)
{
	int old_pt = point();
	Piece before = *grid[place];
	*grid[place] = p;
	G->MW->Set_Piece(p, place);//player info
	int new_pt = point();
	bool changed_pt = (new_pt != old_pt);
    if (changed_pt) { emit changePoint(new_pt); }

	//后续的boss的反应都在这里
}