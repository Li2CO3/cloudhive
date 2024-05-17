#include "gametools.h"
#include "stdafx.h"
#include <QDebug>
#include <QElapsedTimer>
namespace gametools
{
void delay(int timems)
    {
    QElapsedTimer e;
        e.start();
        while(e.elapsed()<timems)
            {}
        return;
    }

};
namespace planetools
{

QString coordText(int X, int Y)
{    return QString(char(X+'A'))+QN(Y); }

Cell_Plane_type plane_occupies(Plane_info i, int x, int y)
{
    if(x==i.centerx && y==i.centery)return (i.type==PlaneType_Normal ? AhhPlane : RDCPlane);
    if(i.type==PT::PlaneType_Normal)
        for(int j=0;j<9;j++)
            {
                if(x==i.centerx+planeposition[i.direction][j][0] && y==i.centery+planeposition[i.direction][j][1])
                    return YesPlane;
            }
    if(i.type==PT::PlaneType_RDCenter)
            for(int j=0;j<8;j++)
            {
            if(x==i.centerx+RDCenterposition[j][0] && y==i.centery+RDCenterposition[j][1])
                return YesPlane;
            }
    return NoPlane;

}
QString cell_string(int x, int y)
{return QString(char('A'-1+x))+QN(y+1);}
QString hint_text(QString h1, int h2, int h3,int h4)
{
    if(h1=="攻击")
            {if(h2==0){return QString("导弹未击中任何飞机。")+(h3>0?QString("您还有")+QN(h3)+"发导弹。":"您已经没有导弹了，本回合结束。");}
            if(h2==1){return QString("导弹命中了飞机！本回合结束。");}
            if(h2==2){return QString("导弹直击飞机要害！！获得额外一回合行动机会，导弹已重新填充。");}
            if(h3==3){return QString("核弹研发中心被击中！研发成功率大幅下降。本回合结束。");}
    }
    if(h1=="攻击首要害")
            {
            return QString("导弹直击飞机要害！！获得额外一回合行动机会，再次命中要害时将不会进行提示。");

        }
    if(h1=="攻击全部")
            {
            if(h2==2){return QString("导弹直击飞机要害！！对方的所有飞机都已被击落！");}
            if(h2==1){return QString("导弹命中了飞机！对方的所有飞机都已被击落！");}
            }
    if(h1=="开始")
            {return QString("当前阶段 >> 第")+QN(h2)+"回合";}
    if(h1=="普通打击")
            {return QString("BOSS进行普通打击：共发射了")+QN(h2)+"枚导弹";}
    if(h1=="打击升级")
            {return QString("【BOSS】普通打击已升级，每回合最多发射 8 枚导弹\n");}
    if(h1=="BOSS1_指挥")
            {if(h2==-1)return "[系统错误] BOSS技能阶段——空军指挥：移动飞机时发生错误，随机放置飞机次数到达上限，已强制中断游戏进程！";
            else return QString("【WARNING】BOSS发动技能 [空军指挥]！已指挥 ") + QN(h2) + " 架飞机飞行至地图的其他位置";
            }
    if(h1=="BOSS1_十字")
            {return QString("【WARNING】BOSS发动技能[连环轰炸]，打击了【")+cell_string(h2,h3)+"】所在的十字区域";}
    if(h1=="BOSS1_55")
    {if(h4>0) return QString("【WARNING】BOSS发动技能 [雷达扫描]，扫描了 ") + cell_string(h2,h3) + " 为中心的 5*5 的区域，并发射" + QN(h4) + " 枚导弹打击了检测到的所有飞机";
            else return QString("【WARNING】BOSS发动技能 [雷达扫描]，扫描了 ") + cell_string(h2,h3) + " 为中心的 5*5 的区域，什么都没有发现";
    }
    if(h1=="BOSS1_33")
    {return QString("【WARNING】BOSS发动技能 [火力打击]！发射了一枚高爆导弹，炸毁了位于 ") + cell_string(h2,h3) + " 的 3*3 的区域";}

    if(h1=="BOSS2_拦截")
    {return QString("【WARNING】BOSS触发技能 [导弹拦截]，当前导弹被拦截并打击到了玩家的地图上");}
    if(h1=="BOSS2_复活")
    {
            if(h2==-2){return QString("[系统错误] BOSS技能阶段——空军支援：随机放置飞机次数到达上限，已强制中断游戏进程！");}
            if(h2==-1){return "【WARNING】BOSS发动技能 [空军支援]！但飞机数已达上限，支援发动失败";}
            if(h2==0){return "【WARNING】BOSS发动技能 [空军支援]！一架新起飞的飞机抵达战场！当前总飞机数为 6";}
            if(h2==1){return "【WARNING】BOSS发动技能 [空军支援]！已成功将一架已被击落的飞机更换为新飞机，并转移位置";}
    }
    if(h1=="BOSS2_快打研发中心")
    {return QString("【NUCLEAR SIREN】核弹研发成功概率已达到 ") + QN(h2/10)+"."+QN(h2%10) + "%，尽快击毁研发中心以拖慢进展";}
    if(h1=="BOSS2_石墨")
            {return "【WARNING】BOSS发动技能 [石墨炸弹]！在本回合，您仅有 1 枚导弹";}
    if(h1=="BOSS2_核弹")
            {auto str = QString("【NUCLEAR SIREN】[核弹] 研发成功并被发射！（概率 ") + QN(h2/10)+"."+QN(h2%10)+"% 已触发）\n";
                   if(h2>=60)str=str+"地图全部区域被摧毁！";
                   else {
                        str=str+(h4?"由于研发受到阻碍，":"由于研发过快，");
                    str=str+"实际威力仅有预期的 "+QN(h3)+"%";
                   }
                   return str;
            }
    if(h1=="结束")
            {if(h2==0){return QString("游戏结束，挑战失败");}
            if(h2==1) {return QString("游戏结束，根据最后一回合行动次数，挑战失败");}
            if(h2==2) {return QString("游戏结束，根据最后一回合行动次数，平局");}
            if(h2==3) {return QString("游戏结束，根据最后一回合行动次数，挑战成功，获得奖励 " + QN(h3) +" 积分！");}
            if(h2==4) {return QString("游戏结束，挑战成功，获得奖励 "+QN(h3)+" 积分！");}
            }
    if(h1.startsWith("奖励"))
            {return QString("/pt transfer @") +h1.mid(2) + " "+QN(h2);}
    return h1+QN(h2)+','+QN(h3);
}

    double missile_bonus_mult(int i)
    {if(i<=2) return 8>>i;
        else return 1.3-0.1*i;
    }
    double checkpoint_bonus_mult(int i)
    {
        return 1.4-i*0.04;
    }

};
