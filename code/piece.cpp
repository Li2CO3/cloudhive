#include "piece.h"
#include <QChar>
#include <QPainter>

Piece::Piece() {
    id = "000";
    x = 0;
    y = 0;
    z = 0;
}

QString inline convert(int x) {
    if (x >= 0 && x <= 9) {
        return QChar('0' + x);
    } else if (x == 10) {
        return QChar('X');
    } else {
        throw 0;//暂时不支持0-10以外的卡牌
    }
}


QPixmap Piece::image() const
{
    QPixmap map(64,64);
    QPainter painter(&map);
    map.fill();
    painter.drawPixmap(0,0,QPixmap("res/piece/pure/part_-" + QString(s159()) + "-.png"));
    painter.drawPixmap(0,0,QPixmap("res/piece/pure/part_--" + QString(s267()) + ".png"));
    painter.drawPixmap(0,0,QPixmap("res/piece/pure/part_" + QString(s348()) + "--.png"));
    //最底下是159 中间是267 最顶层是348
    painter.drawPixmap(0,0,QPixmap("res/piece/pure/edge_pure.png"));
    return map;
}

Piece::Piece(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->id = convert(x) + convert(y) + convert(z);
    if (this->id == "XXX") {
        this->id = "any";
    }
}

int inline convert(QChar x) {
    if (x == 'X') {
        return 10;
    } else if (x >= '0' && x <= '9') {
        return x.unicode() - QChar('0').unicode();
    } else {
        throw 0;//暂时不支持0-9和X以外的卡牌
    }
}


Piece::Piece(QString info) {
    if (info == "any" || info == "XXX") {
        this->x = 10;
        this->y = 10;
        this->z = 10;
        this->id = "any";
    } else {
        this->x = convert(info[0]);
        this->y = convert(info[1]);
        this->z = convert(info[2]);
        this->id = info;
    }
}

bool Piece::isNormalPiece()
{
    if(id == "any") {
        return true;
    }
    if( (x == 3 || x == 4 || x == 8) &&
        (y == 1 || y == 5 || y == 9) &&
        (z == 2 || z == 6 || z == 7)) {
            return true;
    }
    return false;
}

int Piece::ValueofLine(int line) const //返回x线值
{
    if (line == 3 || line == 4 || line == 8)return x348();
    if (line == 2 || line == 6 || line == 7)return x267();
    if (line == 1 || line == 5 || line == 9)return x159();
    throw 0;
}

int Piece::sum() {
    return x348() + x267() + x159();
}
