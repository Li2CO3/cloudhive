#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QFile>
#include <QIODevice>

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

void MainWindow::set_name(QString newname)
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

void MainWindow::set_icon(QString newicon)
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
				//     G->player()->grid.Make_image(pvepage->gridimage);
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


void MainWindow::load_name()
{
    QFile file("res/game/user.txt");
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream inStream(&file);
        content = inStream.readAll();

        name = content.split("\n")[0];
        icon = content.split("\n")[1];
        file.close();
    }
}

MainWindow::~MainWindow()
{
    if(G) G->deleteLater();
}
