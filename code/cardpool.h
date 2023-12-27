#ifndef CRADPOOL_H
#define CRADPOOL_H

#include "stdafx.h"
#include "utils/random.hpp"
#include "utils/gametools.h"
#include "piece.h"


#define MAX_POOL 200//卡池上限 改了没事 就是会影响class Pool大小
#define MAX_XUANXIU 9//给一些容器设置大小。目前只用到2.

class CardPool
{
public:
    int npiece;//卡池张数
    Piece pieces[MAX_POOL];//卡池
    Piece current[MAX_XUANXIU];//当前回合的块
    int ncurrent;//当前回合块数
    CardPool(Random* rd);
    Piece drawout();//卡池里随机移除一张作为返回值
    void pushback(Piece p);//把一张卡放到最后
    int nPool(GT::PoolType type, Piece p);
    void setpool(GT::PoolType type);//根据种类设置卡池

private:
    Random* random;
};

#endif // CRADPOOL_H
