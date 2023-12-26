#ifndef CAT_BURGLAR_H
#define CAT_BURGLAR_H

#include "../monster.h"

class Cat_Burglar :public Game::Monster
{
public:
    Cat_Burglar(Game* G) :Monster(G) { id = Game::CAT_BURGLAR; name = "小贼猫"; shortname = "小贼猫"; initialhealth = 1000; icon = QString("res/icon/monster/") + "3_cat_burglar.jpg"; }
    ~Cat_Burglar() override {}
    void reset() override { Game::Monster::reset(); bomb1 = 0; bomb2 = 0; }//bomb是欺骗技能的位置
    POOLTYPE pooltype() override { return POOLTYPE::POOL_CAT_BURGLAR; }//调色盘
    QString description() override;

    void Monster_Before_Turn() override;//狂风预警，提醒线数，提醒加分，提醒20回合，首回合埋炸弹效果
    void Monster_Before_Combat() override;//加分效果，雷电效果，浓雾效果，炸弹爆炸效果
    void Monster_Combat() override;//浓雾效果
    void Make_Summary(QDialog* dialog) override;//多放狂风的额外块
    int bomb1;
    int bomb2;
};

#endif // CAT_BURGLAR_H
