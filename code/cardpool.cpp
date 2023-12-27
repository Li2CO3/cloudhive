#include "cardpool.h"

#define LAIZI "any"

CardPool::CardPool(Random* rd) {
    this->random = rd;
    npiece = 0;
    setpool(GT::POOL_Normal);
}
Piece CardPool::drawout() {
    int k = random->randint(0, npiece - 1);
    Piece p = pieces[k];
    pieces[k] = pieces[npiece - 1];
    npiece--;
    return p;
}

void CardPool::pushback(Piece p) {
    pieces[npiece] = p;
    npiece++;
    return;
}

int CardPool::nPool(GT::PoolType type, Piece p)//当前仅限正常的块。开放巢的奇怪块打算用另一个函数加
{
    using namespace GT;
    if (p.isNormalPiece())
    {
        switch (type)
        {
        case POOL_Normal:
        case POOL_RAINBOW_GIRAFFE:
            return 2;
        case POOL_TWIN_HEAD:
            if(p==LAIZI) return 2;
            else if (p.x348() == 4)return 0;
            else return 3;
        case POOL_MIMIC_CHEST:
            if(p==LAIZI) return 2;
            if (p.x267() == 6) return 3;
            else return 2;
        case POOL_CAT_BURGLAR:
            if(p==LAIZI) return 5;
            else return 2;
        case POOL_IRON_WALL:
            if(p==LAIZI) return 2;
            else if (p.sum() % 3 && p.sum() % 2 && p.sum() % 5) return 5;//目前就先靠235判定质数了. 412 317 452 416 812 892 496 856 397 896
            else return 2;
        case POOL_PUPPETEER:
            if(p==LAIZI) return 2;
            else if (p.x159() == 1 || p.x267() == 2 || p.x348() == 3) return 3;
            else return 2;
        default:throw 0;
        }
    }
    else
        throw 0;
}
void CardPool::setpool(GT::PoolType type)
{
    int line348[3] = { 3,4,8 };
    int line159[3] = { 1,5,9 };
    int line267[3] = { 2,6,7 };
    this->npiece = 0;
    for (int x348 : line348)
        for (int x159 : line159)
            for (int x267 : line267) {
                Piece p = Piece(x348, x159, x267);
                for (int nthatpiece = 0; nthatpiece < nPool(type, p); nthatpiece++) {
                    this->pushback(p);
                }
            }
    for (int laizis = 0; laizis < nPool(type, Piece(LAIZI)); laizis++)
        this->pushback(Piece(LAIZI));

    // 非正常块
    if (type == GT::POOL_RAINBOW_GIRAFFE) {
        // 图片只有 3X2、4X6、8X7、39X、45X、81X、X16、X57、X92、XXX
        for (int i = 0; i < 1; ++i) {
            this->pushback(Piece("3X2"));
            this->pushback(Piece("4X6"));
            this->pushback(Piece("8X7"));
            this->pushback(Piece("39X"));
            this->pushback(Piece("45X"));
            this->pushback(Piece("81X"));
            this->pushback(Piece("X16"));
            this->pushback(Piece("X57"));
            this->pushback(Piece("X92"));
            this->pushback(Piece("XXX"));
        }
    }
}
