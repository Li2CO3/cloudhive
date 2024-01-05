#ifndef ASSEMBLECRAFTPAGE_H
#define ASSEMBLECRAFTPAGE_H
#include "mainwindow.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "../utils/comb_assembler.hpp"
class AssembleCraftPage :public MainWindow::Page
{
public:
    MainWindow *MW;
    bool chosen[20];
    QPushButton *TheTiles[20];
    QLineEdit * InputKey[20];
    QString piecestr[20];
    QPushButton *QuickInput[28];
    QPushButton * BOOM;//毁灭！
    QPushButton * back;
    QLabel *Title;

    AssembleCraftPage(MainWindow *mw){MW=mw;MW->page=this;for(int i=0;i<20;i++)chosen[i]=false;}
    ~AssembleCraftPage(){}

    void Unchoose(int id)
    {
        chosen[id]=false;
          InputKey[id]->setText("");
        piecestr[id]="";
        TheTiles[id]->setIcon(QIcon(QPixmap("res/piece/pure/num_"+QN(id)+".png").SCALED(128,128)));
    }
    void Choose(QString str, int id=-1)
    {
        if(str=="any")str="XXX";
        if(str.length()>=3 &&
            (QString("0348Xx").contains(str[0]) &&
             QString("0159Xx").contains(str[1]) &&
             QString("0267Xx").contains(str[2]))
            )
            ;
        else return;
        Piece p(str);
        int target=0;
        if(id==-1)
        {
            for(;target<20;target++)
                if(chosen[target]==false)break;
            if(target==20)return;
            id=target;
        }
        chosen[id]=true;
        piecestr[id]=str;
       InputKey[id]->setText(p.to_string());
        TheTiles[id]->setIcon(QIcon(p.image().SCALED(TheTiles[id]->width(),TheTiles[id]->height() )));
        return;

    }
     const QString pieces[28]={"312","412","812","316","416","816","317","417","817",
                                "352","452","852","356","456","856","357","457","857",
                                "392","492","892","396","496","896","397","497","897","any"};
    void load() override
    {
        NEW_LABEL_MW(Title,650,40,300,50,"拼装毁灭者:自选",40);

        for(int i=0;i<20;i++)
        {NEW_BUTTON_IMAGE_MW(TheTiles[i],100+138*(i%10),100+158*(i/10),96,96,"res/piece/pure/num_"+QN(i)+".png");
            QAbstractButton::connect(TheTiles[i],&QPushButton::clicked,MW,[=](){Unchoose(i);});
            }
        for(int i=0;i<20;i++)
            {
            NEW_LINEEDIT_MW(InputKey[i],100+138*(i%10), 200+158*(i/10),96,40,"",24);
            QLineEdit::connect(InputKey[i],&QLineEdit::returnPressed,MW,[=](){Choose(InputKey[i]->text(),i);});
            }
        for(int i=0;i<28;i++)
            {
            NEW_BUTTON_IMAGE_MW(QuickInput[i],100+138*(i/3),500+138*(i%3),96,96,Piece(pieces[i]).image());
            QAbstractButton::connect(QuickInput[i],&QPushButton::clicked, MW,[=](){Choose(pieces[i]);});
            }
        NEW_BUTTON_MW(BOOM,100+138*(28/3),500+138*(28%3),96,96,"毁灭!",30);
        NEW_BUTTON_MW(back,100+138*(29/3),500+138*(29%3),96,96,"返回",30);

        QAbstractButton::connect(BOOM,&QPushButton::clicked,MW,[=](){

            for(int i=0;i<20;i++) if(!chosen[i])return;
            QString str="card ";
            for(int i=0;i<20;i++) str=str+piecestr[i]+' ';
            QDialog *dialog = new QDialog(MW);
            Comb_Assembler::Make_Assemble_Page(str,dialog);
        });
        QAbstractButton::connect(back,&QPushButton::clicked,MW,[=](){MW->load_page(NUMCOMBPREPPAGE);});

    }



};

#endif // ASSEMBLECRAFTPAGE_H
