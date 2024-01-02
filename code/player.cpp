#include "player.h"
#include "game.h"
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

int Player::point()
{
    return grid.point(G->scoring_rule);
}
void Player::take_damage(int dmg)
{
    health -= dmg;
    emit G->Alert_monster(name + "血量减少" + QString::number(dmg));
    emit G->signal_player_change_stats(stat_string());
    if (health <= 0) { dead = true; emit G->Alert_monster(name + "已被击杀..."); G->Game_End(); }
}
void Player::changePoint()
{
    emit G->signal_player_change_stats(stat_string());
}

void Player::Locate_piece(Piece p, int place)
{
    int old_pt = point();
    Piece before = *grid[place];
    *grid[place] = p;
    G->MW->Set_Piece(p, place);//player info
    int new_pt = point();
    bool changed_pt = (new_pt != old_pt);
    if (changed_pt) { emit G->signal_player_change_stats(stat_string()); }

    //后续的boss的反应都在这里
}
