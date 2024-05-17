#ifndef PLANEBATTLEGAME_H
#define PLANEBATTLEGAME_H
#include "mainwindow.h"
#include "planeboss.h"
class PlaneBattleGame : public QObject
{
    Q_OBJECT
public:
    MainWindow*MW;

    PlaneBattleGame(MainWindow *mw){MW=mw;make_basic_connections();}
    ~PlaneBattleGame(){if(boss)delete boss; if(random)delete random;}

    Random *random=nullptr;
    PlaneBoss *boss = nullptr;
    PT::PlaneGame_Boss_id boss_id;
    PT::PlaneGame_Overlap_rule_type overlap_rule;
    PT::PlaneGame_Critical_rule_type critical_rule;
    bool critical_first_used=false;
    int critical_first_x=-1, critical_first_y=-1;
    int n_missile;
    int n_checkpoint;
    QString game_seed;

    int round=0;
    int n_missile_togo;int n_missile_this_turn;
    bool seethrough=true;

    int n_player_last_round=0,n_boss_last_round=0;

    PT::Plane_info marks[100];
    int nmarks=0;

        class Cell
        {
        public:

            PT::Cell_Strike_type strike;
            int strike_turn=-1;

            Cell(){strike=PT::STRIKE_NO; }
        };
    Cell player_grid[14][14];
    Cell boss_grid[14][14];


    PT::Plane_info player_info[3];int n_playerplanes=0;
    PT::Plane_info boss_info[7]; int n_bossplanes=0;

    void add_player_plane(int x, int y, PT::Plane_direction dir)
    {
            player_info[n_playerplanes]=PT::Plane_info(PT::PlaneType_Normal, dir,x,y);
                n_playerplanes++;
            return;
    }
    void add_boss_plane(int x, int y, PT::Plane_direction dir, PT::Plane_type typ=PT::PlaneType_Normal)
    {
            boss_info[n_bossplanes]=PT::Plane_info(typ,dir,x,y);
            n_bossplanes++;
            return;
    }


    PT::Game_Status game_status=PT::Not_Started;
    void set_status(PT::Game_Status t){game_status=t;return;}

    void startPlaneBattle(int bossid, int overlap, int critical, int missiles, int checkpoints, QString seed)
    {
        if(seed=="")seed=QN(time(NULL));
        RandomMaker::make_random(random,GT::MT19937RANDOM,seed);
        game_seed=seed;
        boss_id=PT::PlaneGame_Boss_id(bossid); overlap_rule=PT::PlaneGame_Overlap_rule_type(overlap);
        critical_rule=PT::PlaneGame_Critical_rule_type(critical);
        n_missile=missiles; n_missile_this_turn=n_missile; n_missile_togo=n_missile;
        n_checkpoint=checkpoints;
        //qDebug()<<"checkpoint="<<n_checkpoint;
        Set_checkpoint();
        if(boss)delete boss;
        switch(bossid)
        {
        case 1:
            boss=new Boss_Fire(this);
            break;
        case 2:
            boss=new Boss_Nuke(this);
            break;
        }
        game_status=PT::Put_Planes;
        emit signal_update_grid();
        return;
    }



    void Boss_put_planes()
    {

        boss->put_planes();
    }
    void Set_checkpoint(){

        if(n_checkpoint==100)n_checkpoint=random->randint(0,10);
        for(int i:{0,13})for(int j:{0,13})
            {boss_grid[i][j].strike=PT::STRIKE_YES;
             player_grid[i][j].strike=PT::STRIKE_YES;}

        for(int i=0;i<n_checkpoint;i++)
        {
            int k=random->randint(0,196-4-1-i);
            for(int w=0;w<196;w++)
             if(player_grid[w/14][w%14].strike==PT::STRIKE_NO)
             {if(k==0)
                 {player_grid[w/14][w%14].strike=PT::STRIKE_INSPECT;
                  boss_grid  [w/14][w%14].strike=PT::STRIKE_INSPECT;
                     break;}
                     k--;}
        }
        emit signal_update_grid();
        emit signal_text(QString("本局侦察数：")+QN(n_checkpoint));



    }
    void Player_put_plane(int x, int y, PT::Plane_direction dir){

        add_player_plane(x,y,dir);
        //qDebug()<<x<<y<<int(dir);
        emit signal_update_grid();
        //QObject::connect(this,PlaneBattleGame::)

    }
    void Player_remove_plane(int x, int y,bool emits=true)
    {
        for(int i=0;i<n_playerplanes;i++)
            if(player_info[i].centerx==x && player_info[i].centery==y)
            {
                player_info[i]=player_info[n_playerplanes-1];
                n_playerplanes--;
                if(emits)
                emit signal_update_grid();
                return;
            }
    }
    void Player_remove_ALL_plane(){while(n_playerplanes>0)Player_remove_plane(player_info[0].centerx,player_info[0].centery,false);emit signal_update_grid();}

    bool player_can_put_plane(PT::Plane_info p)
    {
        //if(p.direction==PT::PlaneDir_L)return false;
        if(n_playerplanes==3)return false;
        if(cell_using_status(p.centerx,p.centery,false)!=PT::NoPlane)return false;
        if(player_grid[p.centerx][p.centery].strike!=PT::STRIKE_NO)return false;
        if(overlap_rule==PT::OVERLAP_NO)
            for(int i=0;i<9;i++)
                if(cell_using_status(p.centerx+PT::planeposition[p.direction][i][0],p.centery+PT::planeposition[p.direction][i][1],false)!=PT::NoPlane)
                    return false;
        if(overlap_rule==PT::OVERLAP_YES)
            for(int i=0;i<9;i++)
                if(cell_using_status(p.centerx+PT::planeposition[p.direction][i][0],p.centery+PT::planeposition[p.direction][i][1],false)!=PT::NoPlane &&
                   cell_using_status(p.centerx+PT::planeposition[p.direction][i][0],p.centery+PT::planeposition[p.direction][i][1],false)!=PT::YesPlane )
                    return false;

        return true;
    }
    bool boss_can_put_plane(PT::Plane_info p)
    {
        if(cell_using_status(p.centerx,p.centery,true)!=PT::NoPlane ||
            boss_grid[p.centerx][p.centery].strike==PT::STRIKE_YES ||
            boss_grid[p.centerx][p.centery].strike==PT::STRIKE_INSPECT)
                return false;
        const int (*cellidx)[2] = ( (p.type==PT::PlaneType_RDCenter)?(PT::RDCenterposition):(PT::planeposition[p.direction]));
        int ncell = (p.type==PT::PlaneType_RDCenter ? 8 : 9);
        if(overlap_rule==PT::OVERLAP_NO)
            for(int i=0;i<ncell;i++)
                if(cell_using_status(p.centerx+cellidx[i][0],p.centery+cellidx[i][1],true)!=PT::NoPlane)
                    return false;
        if(overlap_rule==PT::OVERLAP_YES)
            for(int i=0;i<ncell;i++)
                if(cell_using_status(p.centerx+cellidx[i][0],p.centery+cellidx[i][1],true)!=PT::NoPlane &&
                    cell_using_status(p.centerx+cellidx[i][0],p.centery+cellidx[i][1],true)!=PT::YesPlane )
                    return false;

        return true;
    }
    void boss_remove_plane_id(int id, bool emits=true)
    {
        if(id>=n_bossplanes)return;

                boss_info[id]=boss_info[n_bossplanes-1];
                n_bossplanes--;
                if(emits)
                    emit signal_update_grid();
                return;


    }
    PT::Cell_Plane_type cell_using_status(int x, int y,bool isboss)
    {
        if(x<0||x>=14||y<0||y>=14)return PT::AhhPlane;
        if(!isboss)
        for(int i=0;i<n_playerplanes;i++)
        {auto t= plane_occupies(player_info[i],x,y);

            if(t!=PT::NoPlane)return t;
        }
        if(isboss)
        for(int i=0;i<n_bossplanes;i++)
        {auto t= plane_occupies(boss_info[i],x,y);

            if(t!=PT::NoPlane)return t;
        }
        return PT::NoPlane;
    }


    void make_basic_connections()
    {
            connect(this, &PlaneBattleGame::signal_player_put_plane, this, &PlaneBattleGame::Player_put_plane);
            connect(this, &PlaneBattleGame::signal_player_put_planes_done, this, [=]() {
                Boss_put_planes();Start_turn(); signal_update_grid();});
            connect(this, &PlaneBattleGame::signal_player_move, this, [=](int x, int y, int opid) {Do_Players_Plane_Move(x,y,opid);});
            connect(this,&PlaneBattleGame::signal_player_done,this,[=](){
                End_turn();
            });
            //connect(this,&PlaneBattleGame::signal_text,this,[=](QString str){qDebug()<<str;});



    }
    void Do_Players_Plane_Move(int x, int y, int opid){
            set_status(PT::BUSY);
            if(opid==5){
                Player_strike(x,y);
            }
            if(opid>=1 && opid<=4){
                //Player_mark(x,y,PT::Plane_direction(opid));
            }
            return;
    }
    void Start_turn(){
            round++;
            emit signal_text(PT::hint_text("开始",round));
            n_player_last_round=0;
            n_boss_last_round=0;
            n_missile_this_turn=n_missile;
            n_missile_togo=n_missile_this_turn;
            boss->normal_attack();
            boss->special_ability();
            emit signal_update_grid();
            set_status(PT::Game_Status::Combat_Planes);return;
    }

    void Boss_strike(int x, int y)
    {
            if(player_grid[x][y].strike==PT::STRIKE_YES)return;
            player_grid[x][y].strike=PT::STRIKE_YES;
            player_grid[x][y].strike_turn=round;
            return;
    }
    int n_boss_remain()
    {int t=0;
            for(int i=0;i<n_bossplanes;i++){ auto c=boss_info[i]; if(boss_grid[c.centerx][c.centery].strike==PT::STRIKE_NO && c.type==PT::PlaneType_Normal) t++;}
            //qDebug()<<"boss还剩"<<t<<"个头";
            return t;
    }
    int n_player_remain()
    {int t=0;
            for(int i=0;i<n_playerplanes;i++){ auto c=player_info[i]; if(player_grid[c.centerx][c.centery].strike==PT::STRIKE_NO && c.type==PT::PlaneType_Normal) t++;}
            return t;
    }
    void effect_after_strike(PT::Cell_Plane_type c,int x, int y)
    {
            bool isfinish=false;
            {
            if(c==PT::CELL_NO)
                {
                emit signal_text(PT::hint_text("攻击",0,n_missile_togo));

                if(n_missile_togo==0){isfinish=true;}
                else set_status(PT::Combat_Planes);
                }
            else if(c==PT::CELL_YES || (c==PT::CELL_CRITICAL && (critical_rule==PT::CRITICAL_NO_SHOW || (critical_rule==PT::CRITICAL_SHOW_FIRST && critical_first_used))))
                {
                emit signal_text(PT::hint_text("攻击",1));
                isfinish=true;
                if(n_boss_remain()==0)
                {emit signal_text(PT::hint_text("攻击全部",1));}
                }
            else if(c==PT::CELL_CRITICAL)
                {
                if(n_boss_remain()==0)
                {isfinish=true;emit signal_text(PT::hint_text("攻击全部",2));}
                else
                {
                    emit signal_text(PT::hint_text("攻击",2));
                    if(critical_rule==PT::CRITICAL_SHOW_FIRST && !this->critical_first_used)
                    {
                    critical_first_used=true;
                    critical_first_x=x, critical_first_y=y;
                        emit signal_text(PT::hint_text("攻击首要害",0));
                    }
                    n_missile_togo = n_missile_this_turn;

                    set_status(PT::Combat_Planes);
                }
                }
            else //c==PT::CELL_RDCenter
                {
                    emit signal_text(PT::hint_text("攻击",3));
                    isfinish=true;
                }
            }
            emit signal_update_grid();
            if(isfinish)emit signal_player_done();
    }
    void Player_strike(int x,int y)
    {
            if(boss->affect_player_strike(x,y))return;
            set_status(PT::BUSY);
            boss_grid[x][y].strike=PT::STRIKE_YES;
            boss_grid[x][y].strike_turn=round;
            n_player_last_round++;
            n_missile_togo--;

            auto c = cell_using_status(x,y,true);
            effect_after_strike(c,x,y);
    }

    void Player_mark(int x, int y, PT::Plane_direction dir)
    {
                //qDebug()<<"mark:"<<x<<y<<int(dir);
                return;


    }
    int bonusvalue()
    {
                return boss->bonusvalue(overlap_rule,critical_rule,n_missile, n_checkpoint);
    }
    void End_turn(){
                int result=-1;//继续
                if(n_boss_remain()==0)
                {if(n_player_remain()>0)
                    result=4;
                 else
                    {if(n_player_last_round<n_boss_last_round)
                        result=3;
                    else if(n_player_last_round==n_boss_last_round)
                        result=2;
                    else result=1;
                    }
                }
                else if(n_player_remain()==0)
                {result=0;}
        if(result>=0)
                {   if(result<=2)
                    emit signal_text(PT::hint_text("结束",result));
                    if(result>=3)
                    {
                    emit signal_text(PT::hint_text("结束",result,bonusvalue()));
                    emit signal_text(PT::hint_text("奖励"+MW->name,bonusvalue()));
                    }
                set_status(PT::End_Planes);
                    emit signal_update_grid();
                return;
                }
        Start_turn();
    }

    void Clear_marks(){
        if(game_status==PT::Combat_Planes || game_status==PT::End_Planes){nmarks=0;emit signal_update_grid();}
        if(game_status==PT::Put_Planes){Player_remove_ALL_plane();}
    }
    void Mark_plane(int x, int y, PT::Plane_direction dir)
    {
            //qDebug()<<"mark:"<<x<<y<<int(dir);
            marks[nmarks++]=PT::Plane_info(PT::PlaneType_Normal,dir,x,y);
            emit signal_update_grid();
            return;
    }
signals:
    void signal_player_put_plane(int x, int y, PT::Plane_direction dir);
    void signal_player_put_planes_done();
    void signal_boss_move();
    void signal_boss_skill();
    void signal_update_grid();
    void signal_player_move(int x, int y, int opid);
    void signal_player_done();
    void signal_game_end();
    void signal_player_strike(int x, int y);
    void signal_player_remove_plane(int x, int y);
    void signal_player_remove_all_planes();
    void signal_text(QString str);
    //void signal_confirm_move(QString note);


};

#endif // PLANEBATTLEGAME_H
