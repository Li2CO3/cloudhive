#ifndef PIECE_H
#define PIECE_H
#include <QPixmap>
#include "stdafx.h"

class Piece
{
public:

    int x348() const {return isEmptyPiece() ? 0 : x;}//对应方向，返回数值 0~10
    int x159() const {return isEmptyPiece() ? 0 : y;}
    int x267() const {return isEmptyPiece() ? 0 : z;}
    QChar s348() const {if(x348()==10)return 'X'; else return QChar('0'+x348());}//对应方向，返回字符 0~9或'X'
    QChar s159() const {if(x159()==10)return 'X'; else return QChar('0'+x159());}
    QChar s267() const {if(x267()==10)return 'X'; else return QChar('0'+x267());}
    bool isNormalPiece() const;//是不是标准蜂巢的块
    bool isEmptyPiece() const;
    int ValueofLine(int x) const;
    int sum() const;
    Piece();
    Piece(int x, int y, int z) ;
    Piece(QString info) ;
    QString to_string() const ;
    explicit operator QString() const { return this->to_string();};
    explicit operator int() { return sum(); }
    bool operator==(const QString& str) {
        return this->to_string() == (str == "XXX" ? QString("any") : str);
    }

    QPixmap image() const;
    void clear();
private:
    int x, y, z;
};

#endif // PIECE_H
