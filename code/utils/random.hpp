#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>

#include "stdafx.h"


class Random//随机数 摇骰子都在这里
{
private:
    int seed;//mt19937的事情，下次一定，下次一定
    qint64 now;
    Random* random;
public:
    void setseed(int newseed) { seed = newseed; now = seed; }
    int getseed() { return seed; }
    int generate() {//随机生成一个0~32767的数字
        now = 214013 * now;
        now &= 0x7FFFFFFF;
        now = (now + 2531011);

        return (now >> 16) & 0x7FFF;
    }
    int randint(int min, int max)//含上下界
    {
        int range = max - min + 1;
        int a = generate();
        while (a / range == 0x7FFF / range) { a = generate(); }
        return a % range + min;
    }
    Random() { seed = time(NULL); now = seed; }//默认取时间
};

#endif // RANDOM_H
