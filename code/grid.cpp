#include "grid.h"
#include <QPainter>


int Grid::point(GT::SCORING_RULE rule)
{
    using namespace GT;
    /*
        auto effectof = [=](int place, int dir){
            if(place>19)return 0;
            int x=GRID_X[place],y=GRID_Y[place];
            int dx,dy;
            if(dir==3){dx=1;dy=1;}
            else if(dir==2){dx=1;dy=-1;}
            else if(dir==1){dx=0;dy=2;}
            else throw 0;

            int cur_val=pieces[place].ValueofLine(dir);
            //int count_cells=1;
            for(int k=-1;k<=1;k+=2)
                for(int s=1;;s++)
                {int step=k*s;

                int x2=x+step*dx,y2=y+step*dy;
                if(x2>6||x2<0||y2>13||y2<1)break;
                int newid=place_ids[y2][x2];
                if(newid>19)break;
                //count_cells++;
                int newvalue=pieces[newid].ValueofLine(dir);
                if(cur_val==10){cur_val=newvalue;continue;}
                if(newvalue==10)continue;
                if(cur_val==newvalue)continue;
                    return 0;
            }
            return cur_val;
        };
        int pt=0;
        for(int i=1;i<=19;i++)
            for(int dir=1;dir<=3;dir++)
                pt+=effectof(i,dir);
        return pt;
    */
    //int a[2][1]={{1},{2}};
    auto f = [=](int a, int b) {if (a == 10)return b; if (b == 10)return a; if (a == b)return a; return 0; };

    int l267[5][5] = { {1,4,8,-1,-1},{2,5,9,13,-1},{3,6,10,14,17},{7,11,15,18,-1},{12,16,19,-1,-1} };
    int l159[5][5] = { {1,2,3,-1,-1},{4,5,6,7,-1},{8,9,10,11,12},{13,14,15,16,-1},{17,18,19,-1,-1} };
    int l348[5][5] = { {8,13,17,-1,-1},{4,9,14,18,-1},{1,5,10,15,19},{2,6,11,16,-1},{3,7,12,-1,-1} };
    int r267[5] = { 10,10,10,10,10 }, r159[5] = { 10,10,10,10,10 }, r348[5] = { 10,10,10,10,10 };
    for (int l = 0; l < 5; l++)
        for (int p = 0; p < 5; p++)
        {
            int t = l267[l][p]; if (t > 0) r267[l] = f(r267[l], pieces[t].x267());
            t = l159[l][p]; if (t > 0) r159[l] = f(r159[l], pieces[t].x159());
            t = l348[l][p]; if (t > 0) r348[l] = f(r348[l], pieces[t].x348());
        }
    int basic_point = 0; int const coef[5] = { 3,4,5,4,3 };
    for (int i = 0; i < 5; i++) basic_point += (r267[i] + r159[i] + r348[i]) * coef[i];
    int points = basic_point;

    switch (rule)
    {
    case Scoring_Normal:
        return basic_point;
    case One_Lines_Plus_12:
        for (int i = 0; i < 5; i++) if (r159[i] == 1)points += 12; return points;
    case One_Lines_Plus_15:
        for (int i = 0; i < 5; i++) if (r159[i] == 1)points += 15; return points;
    case COUNT_LINES:
        points=0;
        for (int i = 0; i < 5; i++) points += ((r267[i]>0) + (r159[i]>0) + (r348[i]>0));
        return points;
    default:
        throw 0;
    }

}

void Grid::Make_image(QPixmap& img)
{
    using namespace GT;
    img = QPixmap(64 * 7, 64 * 7);
    QPainter painter(&img);


    for (int place = 1; place <= 19; place++)
    {
        if (*(*this)[place] != 0)
            painter.drawPixmap(GRID_X[place] * 64, GRID_Y[place] * 32 - 32, (*this)[place]->image());
        else
            painter.drawPixmap(GRID_X[place] * 64, GRID_Y[place] * 32 - 32, QPixmap("res/piece/"+QString("pure")+"/num_" + QString::number(place) + ".png"));
    }
    auto find_xydir = [=](int x, int y, int dir) {
        int dir_val = -1;
        int dx, dy;
        if (dir == 3) { dx = 1, dy = 1; }
        else if (dir == -3) { dx = -1, dy = -1; }
        else if (dir == 1) { dx = 0; dy = 2; }
        else if (dir == -1) { dx = 0; dy = -2; }
        else if (dir == 2) { dx = 1; dy = -1; }
        else if (dir == -2) { dx = -1; dy = 1; }
        else throw 0;
        for (int k = 1;; k++)
        {
            int targetx = x + k * dx; int targety = y + k * dy;
            if (targetx > 6)break;
            if (targetx < 0)break;
            if (targety < 1)break;
            if (targety > 13)break;
            int targetid = place_ids[targety][targetx];
            if (targetid >= 20)
            {
                if (dir_val != -1) { break; }
                else { continue; }
            }
            int newval;
            if (dir == 3 || dir == -3)newval = (*this)[targetid]->x348();
            if (dir == 2 || dir == -2)newval = (*this)[targetid]->x267();
            if (dir == 1 || dir == -1)newval = (*this)[targetid]->x159();
            if (dir_val == -1 || dir_val == 10)
            {
                dir_val = newval; continue;
            }
            if (newval == 10)continue;
            if (dir_val != newval)return 0;
            continue;
        }
        return dir_val;
    };

    for (int place = 20; place <= 45; place++)
    {
        int x = GRID_X[place], y = GRID_Y[place];
        QString wall_desc = "000";
        if (find_xydir(x, y, 3) > 0 || find_xydir(x, y, -3) > 0) { wall_desc[0] = '1'; }
        if (find_xydir(x, y, 2) > 0 || find_xydir(x, y, -2) > 0) { wall_desc[2] = '1'; }
        if (find_xydir(x, y, 1) > 0 || find_xydir(x, y, -1) > 0) { wall_desc[1] = '1'; }
        //find_xydir(0,2,3);find_xydir(0,2,-3);
        painter.drawPixmap(x * 64, y * 32 - 32, QPixmap("res/piece/"+QString("pure")+"/wall_" + wall_desc + ".png"));
    }

    for (int x = 0; x < 7; x += 2)
    {
        painter.drawPixmap(x * 64, 0, QPixmap("res/piece/"+QString("pure")+"/wall_half.png"));
        painter.drawPixmap(x * 64, 32 * 13, QPixmap("res/piece/"+QString("pure")+"/wall_half.png"));
    }
    //0~6 0~14 center=3,7

    //1,2 2,1 3,2 4,1 5,2
    //    2,3     4,3
    //....
    //1,14 2,13 3,14 4,13 5,14
    //  2,15  4,15
}
