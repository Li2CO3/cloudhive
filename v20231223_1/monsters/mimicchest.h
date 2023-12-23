#ifndef MIMIC_CHEST_H
#define MIMIC_CHEST_H

#include "monster.h"

class Mimic_Chest :public Game::Monster//宝箱怪，全名"会说话的宝箱"太长被碳酸锂自作主张改短了
{
public:
    Mimic_Chest(Game* G);
    ~Mimic_Chest() override {}
    void reset() override;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_MIMIC_CHEST; }
    QString description() override;

    void Monster_Before_Turn() override;//如果有存活，不发了。看块提示即将上几分。
    void Monster_Before_Combat() override;//加分；如果上分了，看看上次是几回合前，叠盾
    void addPoint(int pt) override;//如果破50，马上翻六张
private:
    bool iskeep(Game::Piece piece) { return piece.x159() == 1; }//翻牌是是否保留
    bool ready_for_ability_1;//是否马上就发动翻六张
    Game::Piece future_tiles[6];//翻完牌留下来的
    int nFutureTiles;//翻完留下来的
    int turn_of_last_change;//上次改分是第几回合
    void trigger_ability_1();//发动1技能
    int next_future;//上次翻完留下来的发到第几块了
};

#endif // MIMIC_CHEST_H
