#include "planeboss.h"
#include "planebattlegame.h"


PlaneBoss::PlaneBoss(PlaneBattleGame *pg )
    {PG=pg;random=PG->random;}

void PlaneBoss::put_planes()
{
    int overall_retry=0;
    while(true)
    {
    restart:
        int Wid=14;
        int Hei=14;
        int X,Y;

    int RDToGo = nRDCenter();
    while(RDToGo>0)
    {
        X=random->randint(2,Wid-3);
        Y=random->randint(2,Hei-3);
        if(PG->boss_grid[X][Y].strike!=PT::STRIKE_INSPECT)
            {PG->add_boss_plane(X,Y,PT::PlaneDir_NO,PT::PlaneType_RDCenter);RDToGo--;}
    }
    int PlaneToGo=nPlane();
    while(PlaneToGo>0)
    {
    int trycount=0;
    X = random->randint(0,Wid-1);
    Y = random->randint(0,Hei-1);
    auto direction = PT::Plane_direction(random->randint(1,4));

    PT::Plane_info i(PT::PlaneType_Normal,direction,X,Y);


    if(PG->boss_can_put_plane(i))
        {PG->add_boss_plane(X,Y,direction);trycount=0;PlaneToGo--; if(PlaneToGo==0)goto done;}
    else trycount++;
    if (trycount > 5000)
        {

        trycount = 0;
        overall_retry++;
        while(PG->n_bossplanes>0)
        {
            PG->boss_remove_plane_id(PG->n_bossplanes-1,false);
        }
        goto restart;
        }
    }
    }
    done:
    emit PG->signal_text(QString("Boss已部署(已尝试")+QN(overall_retry)+"次)");
    emit PG->signal_update_grid();
    //qDebug()<<"boss planes done.";
}


void PlaneBoss::normal_attack()
{
    int X, Y;
    string str;
    int try_count = 0, count = 0;
    int num;
    if (PG->round < 18) {
    num = rand() % 4 + 3;
    } else {
    num = rand() % 4 + 5;
    }

    int Wid=14,Hei=14;

    for (int i = 0; i < num; i++) {
        if (try_count > 1000) break;
        X = random->randint(0,Wid-1);
        Y = random->randint(0,Hei-1);

        auto s=PG->player_grid[X][Y].strike;
        if(s==PT::STRIKE_YES || s==PT::STRIKE_INSPECT)
            {     i--; try_count++; continue;}
         else {
            PG->Boss_strike(X,Y);
            count++;
        }
    }

    emit PG->signal_update_grid();
    if(PG->round == 18)
        emit PG->signal_text(PT::hint_text("打击升级",0));
    emit PG->signal_text(PT::hint_text("普通打击",count));

    PG->n_boss_last_round=count;
    return;
}






void Boss_Fire::AirForce_Command()
{

    int n_alive = 0;
    for(int i=PG->n_bossplanes-1;i>=0;i--)
    {
        auto c=PG->boss_info[i];
        if(PG->boss_grid[c.centerx][c.centery].strike == PT::STRIKE_NO)
        {n_alive++; PG->boss_remove_plane_id(i,false);}
    }


    //n_alive=剩余飞机数。能飞的全飞了。

            int try_count = 0;

            int n_togo = n_alive;

            while (n_togo>0) {

        int const Wid=14, Hei=14;

        int X = random->randint(0,Wid-1);
        int Y = random->randint(0,Hei-1);
        auto direction = PT::Plane_direction(random->randint(1,4));

                if(!PG->boss_can_put_plane(PT::Plane_info(PT::PlaneType_Normal, direction,X,Y)))
                    {try_count++;}
                else{
                        int found_count = 0;
                        for (int i = 0; i < 9; i++) {
                        if (PG->boss_grid[X +PT::planeposition[direction][i][0]][Y + PT::planeposition[direction][i][1]].strike > 0) {
                                found_count++;
                            }
                        }
                        bool hide = false;
                        for (int i = 0; i <= 9; i++) {
                            if ((found_count <= i && try_count >= i * 300) || (found_count <= 1 && random->randint(1,80) == 80)) {
                                hide = true; break;
                            }
                        }
                        if (hide) {
                            PG->add_boss_plane(X,Y,direction,PT::PlaneType_Normal); n_togo--; try_count=0;
                        }
                    }
                if (try_count > 3000) {

                    for(int i=0;i<PG->n_bossplanes;i++)
                        PG->Player_strike(PG->boss_info[i].centerx, PG->boss_info[i].centery);
                    //return "[系统错误] BOSS技能阶段——空军指挥：移动 " + to_string(alive_count) + " 架飞机时发生错误，随机放置飞机次数到达上限，已强制中断游戏进程！";
                    emit PG->signal_text(PT::hint_text("BOSS1_指挥",-1));break;
                }
            }
            emit PG->signal_text(PT::hint_text("BOSS1_指挥",n_alive));
            //emit PG->signal_confirm_move("空军指挥");

}
void Boss_Fire::special_ability()
{
    int const skill_prob[4]={15,30,45,70};
    //        int const skill_prob[4]={50,51,52,53};
    int skill = random->randint(1,100);

    int add_p=0;
    if(PG->n_boss_remain() <= nPlane()-3)add_p=5;

    if(skill>100-skill_prob[0]-add_p *1)
    {
        //15%空军指挥
        AirForce_Command();
    }

      if (skill > 100 - skill_prob[0] - add_p * 1)
        {
        }
        else if (skill > 100 - skill_prob[1] - add_p * 2)
        {
        int Wid=14, Hei=14;
            // 15%概率触发连环轰炸，打击十字区域
            int X = random->randint(0,Wid-1) ;
            int Y = random->randint(0,Hei-1);
            for (int i = 0; i < Hei; i++)
                PG->Boss_strike(X,i);

            for (int j=0;j<Wid; j++)
                PG->Boss_strike(j,Y);

            emit PG->signal_text(PT::hint_text("BOSS1_十字",X,Y));
        }
        else if (skill > 100 - skill_prob[2] - add_p * 3)
        {
            int X,Y;
            int Wid=14,Hei=14;
            // 15%概率触发雷达扫描，扫描5*5，击落所有飞机
            int count, exposed_space_count, exposed_plane_count;
            bool remain_head;
            for (int attempt = 1; attempt <= 30; attempt++) {
                count = exposed_space_count = exposed_plane_count = 0;
                remain_head = true;
                X = random->randint(2,Wid-3);
                Y = random->randint(2,Hei-3);
                for (int i = -2; i <= 2; i++) {
                    for (int j = -2; j <= 2; j++) {
                if (PG->player_grid[X+i][Y+j].strike) {
                auto t=PG->cell_using_status(X+i,Y+j,false);
                        if(t==PT::CELL_NO)exposed_space_count++;
                        if(t==PT::CELL_YES) exposed_plane_count++;
                        if(t==PT::CELL_CRITICAL) remain_head = false;

                        }
                    }
                }
                if ((remain_head && exposed_plane_count >= 3) || exposed_space_count <= 10 || attempt == 30) {
                    for (int i = -2; i <= 2; i++) {
                        for (int j = -2; j <= 2; j++) {
                        auto t=PG->cell_using_status(X+i,Y+j,false);
                        if(t!=PT::CELL_NO)
                            {
                            PG->Boss_strike(X+i,Y+j);
                                count++;
                            }
                        }
                    }
                    break;
                }
            }

            emit PG->signal_text(PT::hint_text("BOSS1_55",X,Y,count));

        }
        else if (skill > 100 - skill_prob[3] - add_p * 4)
        {
            int Wid=14,Hei=14;
            int X,Y;
            // 25%概率触高爆导弹，打击3*3区域
            for (int attempt = 1; attempt <= 30; attempt++) {
                int exposed_count = 0;
                X = random->randint(1,12);
                Y = random->randint(1,12);
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (PG->player_grid[X+i][Y+j].strike !=PT::STRIKE_NO) {
                            exposed_count++;
                        }
                    }
                }
                if (exposed_count <= 4 || attempt == 30) {
                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            PG->Boss_strike(X+i,Y+j);
                        }
                    }
                    break;
                }
            }
            emit PG->signal_text(PT::hint_text("BOSS1_33",X,Y));
        }
        return;

}
bool Boss_Nuke:: bRDhit()
{
        //0号是研发中心
        return !(PG->boss_grid[PG->boss_info[0].centerx][PG->boss_info[0].centery].strike==PT::STRIKE_NO);
}

int Boss_Nuke::pNuke()
{
        int p=PG->round*5;
        int W=14, H=14;
        for(int x=0;x<W;x++)
            for(int y=0;y<H;y++)
            {if(PG->boss_grid[x][y].strike==PT::STRIKE_YES)
                {auto t=PG->cell_using_status(x,y,true);
                    if(t==PT::CELL_YES) p+=1;
                    if(t==PT::CELL_CRITICAL) p+=5;
                    if(t==PT::CELL_RDCENTER)p-=100;
                }
                }
        //emit PG->signal_text("爆率为"+QN(p));
        return p;

}
void Boss_Nuke::AirForce_Reinforce()
{

        int deads = 0;
        for(int i=PG->n_bossplanes-1;i>=1;i--)
        {
                auto c=PG->boss_info[i];
                if(PG->boss_grid[c.centerx][c.centery].strike == PT::STRIKE_YES)
                {deads++; }
        }
        if(deads>0)
        {
            int index=random->randint(0,deads-1);
            int id=PG->n_bossplanes-1;
            for(;id>=1;id--)
            {
                    auto c=PG->boss_info[id];
                    if(PG->boss_grid[c.centerx][c.centery].strike == PT::STRIKE_YES)
                    {if(index==0)break; index--;}
            }
            PG->boss_remove_plane_id(id);
        }

        if(deads==0 && this->nPlane()==6)
            {emit PG->signal_text(PT::hint_text("BOSS2_复活",-1));
            return;
            }
        else if(deads==0){nPlane_now++;}

        int try_count = 0;

        int n_togo = 1;

        while (n_togo>0) {

                int const Wid=14, Hei=14;

                int X = random->randint(0,Wid-1);
                int Y = random->randint(0,Hei-1);
                auto direction = PT::Plane_direction(random->randint(1,4));

                if(!PG->boss_can_put_plane(PT::Plane_info(PT::PlaneType_Normal, direction,X,Y)))
                {try_count++;}
                else{
                int found_count = 0;
                for (int i = 0; i < 9; i++) {
                    if (PG->boss_grid[X +PT::planeposition[direction][i][0]][Y + PT::planeposition[direction][i][1]].strike > 0) {
                        found_count++;
                    }
                }
                bool hide = false;
                for (int i = 0; i <= 9; i++) {
                    if ((found_count <= i && try_count >= i * 300) ) {
                        hide = true; break;
                    }
                }
                if (hide) {
                    PG->add_boss_plane(X,Y,direction,PT::PlaneType_Normal); n_togo--; try_count=0;
                }
                }
                if (try_count > 3000) {

                for(int i=0;i<PG->n_bossplanes;i++)
                    PG->Player_strike(PG->boss_info[i].centerx, PG->boss_info[i].centery);
                //return "[系统错误] BOSS技能阶段——空军指挥：移动 " + to_string(alive_count) + " 架飞机时发生错误，随机放置飞机次数到达上限，已强制中断游戏进程！";
                emit PG->signal_text(PT::hint_text("BOSS2_复活",-2));
                break;
                }
        }
        emit PG->signal_text(PT::hint_text("BOSS2_复活",deads>=1));


}
void Boss_Nuke::Graphite_Bomb()
{
        PG->n_missile_this_turn-1;
        PG->n_missile_togo=1;
        emit PG->signal_text(PT::hint_text("BOSS2_石墨",0));
        return;
}
void Boss_Nuke::special_ability()
{
        const int skill_prob[2]={5,20};
        int X,Y;

        int pnuke=pNuke();
        bool isnuke = random->randint(1,1000)<=pnuke;

        if(isnuke && !nuked)
        {
                this->nuked=true;

                int power=100;
                if(pnuke<60)
                {
                power=40+random->randint(0,3)*10;
                }
                emit PG->signal_text(PT::hint_text("BOSS2_核弹",pnuke,power,bRDhit()));

                int const W=14, H=14;
                for(int i=0;i<W;i++)for(int j=0;j<H;j++)
                    if(power==100 || (random->randint(1,100)<=power))
                        PG->Boss_strike(i,j);


        }
        else
        {
                if(pnuke>=60 && !(bRDhit()) && !nuked)
                {
                emit PG->signal_text(PT::hint_text("BOSS2_快打研发中心",pnuke));
                }
        }
        int skill=random->randint(1,100);

        if(skill>100-skill_prob[0])
        {

                AirForce_Reinforce();
        }
        else if(skill>100-skill_prob[1])
        {
                Graphite_Bomb();
        }
}

bool Boss_Nuke::affect_player_strike(int x, int y)
{
        if(random->randint(1,100)>8)return false;
        if(PG->player_grid[x][y].strike!=PT::STRIKE_NO)return false;

            PG->set_status(PT::BUSY);

        emit PG->signal_text(PT::hint_text("BOSS2_拦截",0));

            PG->player_grid[x][y].strike=PT::STRIKE_YES;
            PG->player_grid[x][y].strike_turn=PG->round;
            PG->n_player_last_round++;
            PG->n_missile_togo--;

            auto c = PG->cell_using_status(x,y,false);
            PG->effect_after_strike(c,x,y);

            return true;
}
int Boss_Fire::bonusvalue(int a, int b, int c, int d)
{
            int basic=400;
            if(a)basic+=400;
            if(b==1)basic+=1000;
            if(b==2)basic+=800;
            if(a&&b) basic+=700;
            return basic * PT::missile_bonus_mult(c) * PT::checkpoint_bonus_mult(d)+0.0001;
}
int Boss_Nuke::bonusvalue(int a, int b, int c, int d)
{
            int basic=500;
            if(a)basic+=500;
            if(b==1)basic+=1200;
            if(b==2)basic+=1000;
            if(a&&b) basic+=800;
            return basic * PT::missile_bonus_mult(c) * PT::checkpoint_bonus_mult(d)+0.0001;

}
