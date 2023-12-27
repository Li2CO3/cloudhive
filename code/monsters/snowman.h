#ifndef SNOWMAN_H
#define SNOWMAN_H

#include "stdafx.h"
#include "../monster.h"
#include "grid.h"

class Snowman :public Monster
{
public:
    Snowman(Game* G);
    ~Snowman() override {}
    void reset() override ;

    QString description() override;
    RULE scoringrule() override { return RULE::One_Lines_Plus_15; }
    void Monster_Before_Turn() override;//太乱了。越乱越需要注释，也越不想打注释
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
    void Go_to_illusion() { is_in_illusion = true; point = 100; }
    void Go_out_of_illusion();
    //void Monster_Before_Combat() override{}

    //define new status: WAIT_SNOWMAN_DISCARD
    void addPoint(int pt) override;
    void take_real_damage(int pt);
    void deal_damage(int dmg) override;
    void take_damage(int dmg) override;
    void setStatus() override;
    int cards_discarded;
    bool is_in_illusion;
    int illusion_point;
    int prev_armor;
    int damage_snowman_taken, damage_player_taken;

    Grid turn_20_grid;
    int turn_20_point;
    bool frozen;
    Piece cache_pieces[7];
    QString cache_o[7];
    void Make_Summary(QDialog* dialog) override;
};
#endif // SNOWMAN_H
