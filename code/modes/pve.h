#ifndef PVE_H
#define PVE_H

#include "game.h"
class PveGame : public Game
{
      Q_OBJECT
public:
    PveGame();
    ~PveGame();
    CardPool *& pool(int id=0) override{ Q_UNUSED(id) return pol;}
    Player *& player(int id=0) override{ Q_UNUSED(id) return ply;}
    Monster *& monster(int id=0) override{ Q_UNUSED(id) return monst;}
    void Start() override;
    void Before_Turn() override;
    void Locate_Piece() override;
    void Before_Combat() override;
    void Combat() override;
    void After_Combat() override;
    void recv_operation(QString op) override;
    bool Check_Operation(int ply, QString cache, QString op) override;
    void sync_record() override;
    void load_challenge(GT::MONSTER_ID monst) override;
private:
    CardPool *pol;
    Player * ply;
    Monster * monst;
};


#endif
