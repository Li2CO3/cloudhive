#ifndef PIECE_H
#define PIECE_H
#include <QPixmap>
#include "stdafx.h"

class Piece
{
public:

    int x348() const {return x;}//对应方向，返回数值 0~10
    int x159() const {return y;}
    int x267() const {return z;}
    QChar s348() const {if(x348()==10)return 'X'; else return QChar('0'+x348());}//对应方向，返回字符 0~9或'X'
    QChar s159() const {if(x159()==10)return 'X'; else return QChar('0'+x159());}
    QChar s267() const {if(x267()==10)return 'X'; else return QChar('0'+x267());}
    bool isNormalPiece();//是不是标准蜂巢的块
    bool isEmptyPiece(){return *this=="000";}//000和空应该需要区分
    int ValueofLine(int x) const;
    int sum();
    Piece();
    Piece(int x, int y, int z) ;
    Piece(QString info) ;
    QString to_string() {return id;}
    explicit operator int() { return sum(); }
    explicit operator QString() {return id;}
    bool operator==(const QString& str) {
        return this->id == (str=="XXX"?QString("any"):str);
    }

    bool operator ==(int a) = delete;

    QPixmap image() const;
    void clear() {
       id = "000";
       x = 0;
       y = 0;
       z = 0;
    }
private:
    int x, y, z;
    QString id;
};

#endif // PIECE_H
