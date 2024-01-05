#ifndef NUMCOMB_H
#define NUMCOMB_H

#include "game.h"
#include "player.h"
#include "cardpool.h"
class NumcombGame : public Game
{
    Q_OBJECT
public:
    NumcombGame();
    ~NumcombGame();
    CardPool * pool(int id=0) override{ Q_UNUSED(id) return pol;}
    Player * player(int id=0) override{ return plys[id];}
    //Monster * monster(int id=0) override{ Q_UNUSED(id) return NULL;}
    void setPoolLink(CardPool *p, unsigned int id=0) override{assert(id==0);pol=p;}
    void setPlayerLink(Player *p, unsigned int id=0) override{assert(id<NUMCOMB_MAX_PLAYER);plys[id]=p;}

    void make_basic_connections() override{
        connect(this, &Game::signal_before_turn, this, [=]() {Before_Turn(); });
        connect(this, &Game::signal_turn, this, [=]() {Locate_Piece(); });
    //    connect(this, &Game::signal_before_combat, this, [=]() {Before_Combat(); });
    //    connect(this, &Game::signal_combat, this, [=]() {Combat(); });
    //    connect(this, &Game::signal_after_combat, this, [=]() {After_Combat(); });
    //    connect(this,&Game::signal_before{Before_Turn();});
    }

    void Start(QString seed) override;
    void InitializePool();
    static void Setup_Numcomb_Pool(CardPool *pool, Random* random);
    void Before_Turn() override;
    void Locate_Piece() override{ emit signal_wait_for_operation(); status=WAIT_TURN;return;}
    void recv_operation(QString op) override;
    bool Check_Operation(int ply, QString cache, QString op) override;//{return false;}
    void sync_record() override
        {//TODO
            record.npieces[turn] = pool()->ncurrent;
            for (int i = 0; i < pool()->ncurrent; i++)record.pieces[turn][i] = pool()->current[i];
        }
    int nplayer(){return nply;}
private:
    int const nply=1;//目前只能是1
    CardPool *pol;
    Player *plys[NUMCOMB_MAX_PLAYER];
    Monster * monst;
};


#endif
