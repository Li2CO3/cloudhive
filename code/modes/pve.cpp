#include "pve.h"
#include "../game.h"
#include "cardpool.h"
#include "../player.h"
#include "../monster.h"
#include "../mainwindow.h"
PveGame::PveGame():Game()
{
    gametype=GT::PVEGAME;
    PveGame::pool() = new CardPool(&this->random);
    PveGame::player() = new Player(this);
    PveGame::monster() = new Monster(this);

}
PveGame::~PveGame()
{
    if(pol)delete pol;
    if(ply) delete ply;
    if(monst) delete monst;
}

void PveGame::sync_record() {//TODO
    record.npieces[turn] = pool()->ncurrent;
    for (int i = 0; i < pool()->ncurrent; i++)record.pieces[turn][i] = pool()->current[i];
}


void PveGame::load_challenge(GT::MONSTER_ID monst)
{
    //pool=new Pool(this);
    //player=new Player(this);
    //现在pool和player没什么特殊的。如果需要可以参照monster

    Monster::new_monster(monst, this);
    scoring_rule = monster()->scoringrule();
    pool()->setpool(monster()->pooltype());
    player()->health = 150;
    return;
}

void PveGame::Start()
{

    status = BUSY;
    monster()->reset();
    player()->reset();


    Before_Turn();
}

void PveGame::Before_Turn()
{
    player()->prev_point = player()->point();
    if (turn == MAX_TURN) { Game_End(); return; }
    turn++;

    monster()->Monster_Before_Turn();


    //qDebug()<<"status="<<status;
    if (status == BUSY)
        emit signal_turn();
}

void PveGame::Locate_Piece()
{
    //qDebug()<<"locate";
    //switch mode
    monster()->setStatus();
    emit signal_wait_for_operation();
    return;
}

void PveGame::Before_Combat()
{

    monster()->Monster_Before_Combat();


    //qDebug()<<"status="<<status;
    if (status == BUSY)
        emit signal_combat();
}

void PveGame::Combat()
{
    //qDebug()<<"combat";

    monster()->Monster_Combat();

    //qDebug()<<"status="<<status;
    if (status == BUSY)
        emit signal_after_combat();
}

void PveGame::After_Combat()
{
    //qDebug()<<"aftercombat";

    //qDebug()<<"status="<<status;

    monster()->Monster_After_Combat();

    if (status == BUSY)
        emit signal_before_turn();
}


void PveGame::recv_operation(QString expr)
{
    if (expr == "QUIT")
    {
        //quit();
        remake(); return;
    }

    if (status >= 0)return;//NOT WAITING
    if (expr.count(",") > 1)throw 0;
    QString cache = expr.split(",")[0];
    QString op = expr.split(",")[1];

    Player* ply = player();
    Piece p;
    if (status == WAIT_TURN)p = pool()->current[0];
    else if (status == WAIT_CHOOSE_TURN)p = pool()->current[cache.toInt()];
    else if (status == WAIT_SNOWMAN_DISCARD) p = Piece(0, 0, 0);
    int place = op.toInt();

    record.covered[turn] = ply->grid.pieces[place];
    ply->Locate_piece(p, place);
    status = BUSY;
    record.cache[turn] = cache;
    record[turn] = op;
    emit signal_before_combat();

}

bool PveGame::Check_Operation(int ply, QString cache, QString op)
{
    Q_UNUSED(ply)
    //switch(mode)
    {
        //default:
        switch (status)
        {
        case WAIT_TURN:
            return true;
            break;
        case WAIT_CHOOSE_TURN:
            return cache != "";
            break;
        case WAIT_SNOWMAN_DISCARD:
            for (int place = 1; place <= 19; place++)if (! (player()->grid.pieces[place].isEmptyPiece()) )//
                    return (op.toInt() != 0) && (! (player()->grid.pieces[op.toInt()] .isEmptyPiece()) );
            return true;
        default:
            return false;
        }
    }

    return true;
}
