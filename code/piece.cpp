#include "piece.h"
#include <QChar>
#include <QPainter>
#include <climits>

Piece::Piece() {
    clear();
}

QString inline convert(int x) {
    if (x >= 0 && x <= 9) {
        return QChar('0' + x);
    }
    else if (x == 10) {
        return QChar('X');
    }
    else {
        throw 0;//暂时不支持0-10以外的卡牌
    }
}

QPixmap Piece::image() const
{
    QPixmap map(64, 64);
    QPainter painter(&map);
    map.fill();
    if (this->isEmptyPiece()) {
        // 空卡牌 暂时图片
        painter.drawPixmap(0, 0, QPixmap("res/icon/diu312.png"));
        return map;
    }
    painter.drawPixmap(0, 0, QPixmap("res/piece/pure/part_-" + QString(s159()) + "-.png"));
    painter.drawPixmap(0, 0, QPixmap("res/piece/pure/part_--" + QString(s267()) + ".png"));
    painter.drawPixmap(0, 0, QPixmap("res/piece/pure/part_" + QString(s348()) + "--.png"));
    //最底下是159 中间是267 最顶层是348
    painter.drawPixmap(0, 0, QPixmap("res/piece/pure/edge_pure.png"));
    return map;
}

Piece::Piece(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

int inline convert(QChar x) {
    if (x == 'X' || x == 'x') {
        return 10;
    }
    else if (x >= '0' && x <= '9') {
        return x.unicode() - QChar('0').unicode();
    }
    else {
        throw 0;//暂时不支持0-9和X以外的卡牌
    }
}

Piece::Piece(QString info) {
    if (info == "any" || info == "XXX") {
        this->x = 10;
        this->y = 10;
        this->z = 10;
    }
    else {
        if (info.length() >= 3 &&
            QString("0348Xx").contains(info[0]) &&
            QString("0159Xx").contains(info[1]) &&
            QString("0267Xx").contains(info[2]))
        {
            this->x = convert(info[0]);
            this->y = convert(info[1]);
            this->z = convert(info[2]);
        }
        else {
            // 如果不符合 暂时初始化为空
            clear();
        }
    }
}

bool Piece::isNormalPiece() const
{
    if (x == 10 && y == 10 && z == 10) {
        return true;
    }
    if ((x == 3 || x == 4 || x == 8) &&
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

int Piece::sum() const {
    return x348() + x267() + x159();
}

bool Piece::isEmptyPiece() const {
    return x == INT_MIN && y == INT_MIN && z == INT_MIN;
}

void Piece::clear()
{
    x = INT_MIN;
    y = INT_MIN;
    z = INT_MIN;
}

QString Piece::to_string() const
{
    if (isEmptyPiece()) {
        return "";
    }
    QString str=convert(x348()) + convert(x159()) + convert(x267());
    return ((str=="XXX") ? "any": str);
}
