#ifndef PUPPETEER_H
#define PUPPETEER_H

#include "../monster.h"

class Puppeteer : public Game::Monster
{
public:
    Puppeteer(Game* G);
    ~Puppeteer() override {}
    void reset() override;
    QString description() override;
    GT::PoolType pooltype() override{return GT::POOL_PUPPETEER;} // 卡牌类型
    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
private:
    int sham;
    int prev_nlines;
};

#endif // PUPPETEER_H
