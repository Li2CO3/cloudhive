#ifndef PIECE_H
#define PIECE_H
#include <QPixmap>
#include "stdafx.h"

class Piece
{
public:

    int x348() const {return x;}
    int x159() const {return y;}
    int x267() const {return z;}
    QChar s348() const {if(x348()==10)return 'X'; else return QChar('0'+x348());}
    QChar s159() const {if(x159()==10)return 'X'; else return QChar('0'+x159());}
    QChar s267() const {if(x267()==10)return 'X'; else return QChar('0'+x267());}
    bool isNormalPiece();
    int ValueofLine(int x) const;
    int sum();
    Piece();
    Piece(int x, int y, int z) ;
    Piece(QString info) ;
    QString to_string() {return id;}
    operator int() { return sum(); }
    operator QString() {return id;}
    bool operator==(const QString& str) {
        return this->id == str;
    }
    bool operator==(const int num) {
        return this->sum() == num;
    }
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
