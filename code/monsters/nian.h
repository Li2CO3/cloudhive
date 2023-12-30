#ifndef NIAN_H
#define NIAN_H

#include "../monster.h"

class Nian_Monster : public Monster
{
public:
    Nian_Monster(Game* G);
    ~Nian_Monster(){}

    void reset() override;
    QString description() override;
    QString stat_string() override;


    void Monster_Before_Turn() override;
    void Monster_Before_Combat() override;
    void Monster_Combat() override;
    //void Monster_After_Combat() override;
    POOLTYPE pooltype() override { return POOLTYPE::POOL_NIAN; }

    void addChill(int chil);
    void Chill_Combat();
private:
    bool has_located[20]={false};//甩炮
    int v159[5]={0},v267[5]={0},v348[5]={0};
    int chill=0;//寒气值，负数对应暖意
    int next_nline_trigger=5;
};
#ifdef NewYear2024NianEasy
class Nian_Easy :public Nian_Monster
{
public:
    Nian_Easy(Game *G);
    ~Nian_Easy(){}
    QString description() override {
        QString str = "2024快乐副本："+ this->name + " 初始血量:" + QN(initialhealth) + "\n";
        str +="副本环境:超级调色盘\n年兽规则基础上，增加18个赖子(共计20个纯赖子和27个单线赖子)。\n其它规则见[年兽]页。";
        return str;
        }
    POOLTYPE pooltype() override { return POOLTYPE::POOL_NIAN_EASY; }
};
#endif//2024快乐年

#endif // NIAN_H
