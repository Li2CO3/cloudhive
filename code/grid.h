#ifndef GRID_H
#define GRID_H
#include "stdafx.h"
#include <QPixmap>
#include "piece.h"
#include "utils/gametools.h"

class Grid//盘面
{
public:
    Piece pieces[20];//1~19是1~19号位。0号是凑数的
    //int prev_point;//上回合分数，boss们曾经会看，遗留产物我直接注释掉
    int point() { return point(GT::Scoring_Normal); };//计分
    int point(GT::SCORING_RULE rule, int *o159=NULL,int *o267=NULL, int *o348=NULL);//计分，带规则. TODO 临时方案：用r159 267 348来输出每条线的连接情况。
    int nlines(){return point(GT::COUNT_LINES);}//计算线数 用开放巢的方式做的
    Piece* operator[](int t) { return &pieces[t]; }//返回盘面几号位。这个感觉可以去掉
    Grid() { Clear(); }
    void Clear() { for (int i = 0; i < 20; i++)pieces[i].clear(); }//清除上回合分数和盘面

    void Make_image(QPixmap& pixmap);//画成图。用LGTBot的图片资料
};

#endif // GRID_H
