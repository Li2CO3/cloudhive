#ifndef CRADPOOL_H
#define CRADPOOL_H

#include "stdafx.h"
#include "utils/random.hpp"
#include "utils/gametools.h"
#include "piece.h"
#include "game.h"

#define MAX_POOL 200//卡池上限 改了没事 就是会影响class Pool大小
#define MAX_XUANXIU 9//给一些容器设置大小。目前只用到2.

class CardPool
{
public:
    Game *G;
    int npiece;//卡池张数
    Piece pieces[MAX_POOL];//卡池
    Piece current[MAX_XUANXIU];//当前回合的块
    int ncurrent;//当前回合块数
    CardPool(GT::PoolType type){setpool(type);}
    CardPool(Game *G);
    Piece random_draw();//卡池里随机移除一张作为返回值。最后一张移动到空位
    Piece draw_top();//抽取0号。后面的平移。
    void pushback(Piece p);//把一张卡放到最后
    int nPool(GT::PoolType type, Piece p);
    void setpool(GT::PoolType type);//根据种类设置卡池
};

#endif // CRADPOOL_H
