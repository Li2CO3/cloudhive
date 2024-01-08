#ifndef PLAYER_H
#define PLAYER_H

#include "grid.h"
#include <QString>
class Game;


    class Player//玩家
    {
    public:
        int id=0;//临时方案
    //TODO:现在id只用在statstring，并且assert id==0。并没有多人模式
        Game* G;
        QString name;//名字
        QString icon;//头像的连接。TODO 现在格式和Monster的不一致。因为做选怪页面先把怪的给改了
        Grid grid;
        int health;
        bool dead;//如果死了就变true
        int totaldamage;//玩家打出的伤害
        int prev_point;//上轮分数
        QString stat_string();//可能是临时方案
        int point();//里面注释掉的版本是适配开放巢的，但不方便设置1加分，就先注释掉了
        void Locate_piece(Piece p, int place);//放块，更改Player里的东西，向Window发出更改显示的signal
        Player(Game* g) { G = g; Clear(); }
        void reset() { totaldamage = 0; prev_point = 0; dead = false; health = 150; grid = Grid(); }//开局状态。如果需要改可能还得跳转到Monster::去
        void Clear() { name = "挑战者"; icon = "lgtbot.jpg"; reset(); }//除了构造函数，没有别处调用



        void changePoint(); //没有point变量。只是发一个信号
        void take_damage(int dmg);//默认的受到伤害

    };
#endif // PLAYER_H
