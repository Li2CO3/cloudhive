#include "mainwindow.h"
//#include "ui_mainwindow.h"

int rel(int x) { return int((x * TheWindow->width()) / 3200) * 2; }

MainWindow* TheWindow;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)

{
	TheWindow = this;
	setWindowTitle("副本之巢");
	setWindowIcon(QPixmap("res/game/gameicon.jpg"));
	setCentralWidget(new QWidget(this));
	status = WAITING;
}

void MainWindow::Set_Piece(Piece p, int place)
{
	Q_UNUSED(p)
    Q_UNUSED(place)

		//qDebug()<<"pagetype="<<pagetype<<"===";
		//switch(pagetype)
	{
		//case PVEPAGE:
		{
			//if(place>0)
			{
				page->update();
				//     G->player->grid.Make_image(pvepage->gridimage);
				//pvepage->grid_background->setPixmap(pvepage->gridimage.SCALED(pvepage->grid_background->width(),pvepage->grid_background->height()));
			}

			connect(G, &Game::signal_wait_for_operation, this, [=]() {QTimer::singleShot(500, this, [=]() {status = WAITING; }); });
			connect(this, &MainWindow::signal_made_operation, this, [=]() {status = NOT_WAITING; }, Qt::DirectConnection);
		}
		//    break;
		//default:throw 0;
	}
	return;
}

MainWindow::~MainWindow()
{
    if(G) G->deleteLater();
}
