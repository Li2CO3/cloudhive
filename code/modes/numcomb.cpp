#include "numcomb.h"

NumcombGame::NumcombGame():Game()
{
    gametype=GT::NUMCOMBGAME;
    NumcombGame::setPoolLink(new CardPool(this));
    for(int i=0;i<NUMCOMB_MAX_PLAYER;i++) NumcombGame::setPlayerLink( new Player(this));
    random=new SimpleRandom();
    scoring_rule=GT::SCORING_NUMCOMB;
    make_basic_connections();

}

NumcombGame::~NumcombGame()
{
    if(pol){delete pol;pol=NULL;}
    for(int i=0;i<nplayer();i++)if(plys[i]) delete plys[i];
}

void NumcombGame::Start(QString seed)
{

    this->setrandom(seed);
    InitializePool();
    this->status=BUSY;
    Before_Turn();
}
void NumcombGame::InitializePool()//包括排序，跳过非赖子。注意：当前未考虑1赖子的情况。此时最后一次抽取swap会不一样。此时总默认跳过非赖子20.
{
    Setup_Numcomb_Pool(pool(), random);
}
void NumcombGame::Setup_Numcomb_Pool(CardPool *pool, Random *random)
{
    pool->setpool(GT::POOL_Normal);
    int id[56]; for(int i=0;i<56;i++) id[i]=i;

    for (int nextpos=0;nextpos<56;nextpos+=2)
    {
        int range=(nextpos+1)*(nextpos+2)-1;
        int draw = random->randint(0,range);

        int swap1 = draw / (nextpos+2);
        int swap2 = draw % (nextpos+2);
        Piece temp;int t;
        {temp =pool->pieces[nextpos];
            pool->pieces[nextpos] = pool->pieces[swap1];
            pool->pieces[swap1]=temp;

            t=id[nextpos]; id[nextpos]=id[swap1]; id[swap1]=t;
            t=id[nextpos+1]; id[nextpos+1]=id[swap2];id[swap2]=t;


            temp =pool->pieces[nextpos+1];
            pool->pieces[nextpos+1] = pool->pieces[swap2];
            pool->pieces[swap2]=temp;
        }
    }

    //for(int i=0;i<56;i+=8)
    //qDebug()<<id[i]<<id[i+1]<<id[i+2]<<id[i+3]<<id[i+4]<<id[i+5]<<id[i+6]<<id[i+7];
    bool skip_non_laizi =true;
    for(int i=0;i<20;i++)
    {if(pool->pieces[i]==LAIZI)
        {skip_non_laizi=false;break;}
    }
    if(skip_non_laizi)
    {
        Piece temp[20];
        for(int i=0;i<20;i++)
            temp[i]=pool->pieces[i];
        for(int i=0;i<54-20+2;i++)
            pool->pieces[i]=pool->pieces[i+20];
        for(int i=0;i<20;i++)
            pool->pieces[54-20+2+i]=temp[i];
    }
}
void NumcombGame::Before_Turn()
{

    {
        for(int plyid=0;plyid<nplayer();plyid++)
        player(plyid)->prev_point = player(plyid)->point();
        if (turn == NUMCOMB_MAX_TURN) { Game_End(); return; }
        turn++;


        pool()->ncurrent = 1;
        Piece t = pool()->draw_top();
        pool()->current[0] = t;

        sync_record();
        emit signal_update_turn_piece();



        //qDebug()<<"status="<<status;
        if (status == BUSY)
            emit signal_turn();
    }
}



void NumcombGame::recv_operation(QString expr)
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
    emit signal_before_turn();

}

bool NumcombGame::Check_Operation(int ply, QString cache, QString op)
{
    Q_UNUSED(ply)
    //switch(mode)
    {
        //default:
        switch (status)
        {
        case WAIT_TURN:
            if(!( player()->grid.pieces[op.toInt()] .isEmptyPiece() )) return false;
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

/*
//void Before_Combat() override;
//void Combat() override;
//void After_Combat() override;
void recv_operation(QString op) override;
bool Check_Operation(int ply, QString cache, QString op) override;
void sync_record() override;
//void load_challenge(GT::MONSTER_ID monst) override;
private:
CardPool *pol;
Player *plys[MAX_NUMCOMB_PLAYER];
Monster * monst;
};
*/
