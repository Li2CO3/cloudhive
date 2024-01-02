#ifndef GAMETOOLS_H
#define GAMETOOLS_H
//...|#26|...|#32|...|#36|...
//#20|...|#30|...|#34|...|#40
//...|#27|...|--8|...|#37|...
//#21|...|--4|...|-13|...|#41
//...|--1|...|--9|...|-17|...
//#22|...|--5|...|-14|...|#42
//...|--2|...|-10|...|-18|...
//#23|...|--6|...|-15|...|#43
//...|--3|...|-11|...|-19|...
//#24|...|--7|...|-16|...|#44
//...|#28|...|-12|...|#38|...
//#25|...|#31|...|#35|...|#45
//...|#29|...|#33|...|#39|...

#define GT gametools
namespace gametools
{

double const GRID_X[46] = { 5,1,1,1, 2,2,2,2, 3,3,3,3,3, 4,4,4,4, 5,5,5,
                           0,0,0,0,0,0, 1,1,1,1, 2,2,3,3,4,4, 5,5,5,5, 6,6,6,6,6,6 };
int const GRID_Y[46] = { 1,5,7,9, 4,6,8,10, 3,5,7,9,11, 4,6,8,10, 5,7,9,
                        2,4,6,8,10,12, 1,3,11,13, 2,12, 1,13, 2,12, 1,3,11,13, 2,4,6,8,10,12 };

int const linelength[5]={3,4,5,4,3};

int const place_ids[15][7] = {
    //0  1  2  3  4  5  6
    { 0, 0, 0, 0, 0, 0, 0},//0
    { 0,26, 0,32, 0,36, 0},//1
    {20, 0,30, 0,34, 0,40},//2
    { 0,27, 0, 8, 0,37, 0},//3
    {21, 0, 4, 0,13, 0,41},//4
    { 0, 1, 0, 9, 0,17, 0},//5
    {22, 0, 5, 0,14, 0,42},//6
    { 0, 2, 0,10, 0,18, 0},//7
    {23, 0, 6, 0,15, 0,43},//8
    { 0, 3, 0,11, 0,19, 0},//9
    {24, 0, 7, 0,16, 0,44},//10
    { 0,28, 0,12, 0,38, 0},//11
    {25, 0,31, 0,35, 0,45},//12
    { 0,29, 0,33, 0,39, 0},//13
    { 0, 0, 0, 0, 0, 0, 0},//14
};//给开放世界巢做准备→ →

enum SCORING_RULE {
    Scoring_Normal = 0,
    One_Lines_Plus_12 = 112,
    One_Lines_Plus_15 = 115,
    COUNT_LAIZI=-2147483646,
    COUNT_LINES=-2147483647,
};

enum PoolType {// 以下枚举符合多种乱写规律
    POOL_Normal=0,
    POOL_TWIN_HEAD=-4,
    POOL_MIMIC_CHEST=66,
    POOL_CAT_BURGLAR=1000,
    POOL_IRON_WALL=235711,
    POOL_PUPPETEER=123,
    POOL_RAINBOW_GIRAFFE=1111,  // 我也不知道为什么是1111
    POOL_NIAN=1112,
    POOL_NIAN_EASY=1113,
    POOL_UNDEAD_DEMON=-199,     // 为什么呢？
};
//setpool

enum GAME_RESULT {
    PLAYER_DEAD = 0,
    TURNS_ENOUGH = 1,
};//UNUSED


enum MONSTER_ID {
    DEFAULT_MONSTER = 0, TWIN_HEAD = 1, SHUANGTOULONG = 1,
    MIMIC_CHEST = 2, BAOXIANGGUAI = 2, HUISHUOHUADEBAOXIANG = 2,
    NAMI = 3, CAT_BURGLAR = 3, XIAOZEIMAO = 3, HAIZEIWANG = 3,
    SNOWMAN = 4, XUEGUAI = 4,
    IRON_WALL = 5,
    STALL = 6,
    PUPPETEER = 7,
    RAINBOW_GIRAFFE = 8,
    NIAN_MONSTER = 9, NIAN_EASY = 2024201000,//2024年 20个赖子
    UNDEAD_DEMON = 10,
};

};
#endif // GAMETOOLS_H
