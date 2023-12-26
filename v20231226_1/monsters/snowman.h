#ifndef SNOWMAN_H
#define SNOWMAN_H

#include "../monster.h"

class Snowman :public Game::Monster
{
public:
    Snowman(Game* G) :Monster(G) {
        id = Game::SNOWMAN;
        name = "雪人"; shortname = "雪人";
        initialhealth = 2888;
        icon = QString("res/icon/monster/") + "4_snowman.jpg";
        }
    ~Snowman() override {}
    void reset() override {
        Game::Monster::reset();
        frozen = false; illusion_point = 0;
        damage_snowman_taken = 0, damage_player_taken = 0;
        is_in_illusion = false; cards_discarded = 0;
    }

    QString description() override;
    RULE scoringrule() override { return RULE::One_Lines_Plus_15; }
    void Monster_Before_Turn() override;//太乱了。越乱越需要注释，也越不想打注释
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    void Monster_After_Combat() override;
    void Go_to_illusion() { is_in_illusion = true; point = 100; }
    void Go_out_of_illusion() {
        is_in_illusion = false;
        emit G->Alert_monster("雪仗结束，结算伤害！");
        take_damage(damage_snowman_taken);
        G->player->take_damage(damage_player_taken);
        G->turn = 21;
        G->player->grid = turn_20_grid;
        G->player->prev_point = G->player->point();
        point = turn_20_point;
        G->MW->page->update();
    }
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

    Game::Grid turn_20_grid;
    int turn_20_point;
    bool frozen;
    Game::Piece cache_pieces[7];
    QString cache_o[7];
    void Make_Summary(QDialog* dialog) override;
};
#endif // SNOWMAN_H
