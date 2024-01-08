#ifndef PVE_H
#define PVE_H

#include "game.h"
#include "player.h"//temp
class PveGame : public Game
{
      Q_OBJECT
public:
    PveGame();
    ~PveGame();
    CardPool * pool(int id=0) override{ Q_UNUSED(id) return pol;}
    Player * player(int id=0) override{ Q_UNUSED(id) return ply;}
    Monster * monster(int id=0) override{ Q_UNUSED(id) return monst;}

    QString player_stat_string(int playerid) override{assert(playerid==0); return QN(player(playerid)->point())+'/'+QN(player(playerid)->health);}//temp

    void setPoolLink(CardPool *p, unsigned int id=0) override{assert(id==0); pol=p;}
    void setPlayerLink(Player *p, unsigned int id=0) override{assert(id==0);ply=p;}
    void setMonster(Monster *m, unsigned int id=0) override{assert(id==0);monst=m;}
    void make_basic_connections() override{
        connect(this, &Game::signal_before_turn, this, [=]() {Before_Turn(); });
        connect(this, &Game::signal_turn, this, [=]() {Locate_Piece(); });
        connect(this, &Game::signal_before_combat, this, [=]() {Before_Combat(); });
        connect(this, &Game::signal_combat, this, [=]() {Combat(); });
        connect(this, &Game::signal_after_combat, this, [=]() {After_Combat(); });
    }
    void Start(QString str) override;
    void Before_Turn() override;
    void Locate_Piece() override;
    void Before_Combat() override;
    void Combat() override;
    void After_Combat() override;
    void recv_operation(QString op) override;
    bool Check_Operation(int ply, QString cache, QString op) override;
    void sync_record() override;
    void load_challenge(GT::MONSTER_ID monst) override;
    //void setrandom(QString seed) override;

private:
    CardPool *pol;
    Player * ply;
    Monster * monst;
};


#endif
