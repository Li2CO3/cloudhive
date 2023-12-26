#ifndef PIECE_H
#define PIECE_H

#include "stdafx.h"

class Piece
{
public:

    int x348() const {return x;}
    int x159() const {return y;}
    int x267() const {return z;}
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
