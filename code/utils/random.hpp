#ifndef RANDOM_H
#define RANDOM_H

#include <QtGlobal>
#include <QStringList>
#include <random>
#include "gametools.h"
#include "stdafx.h"


class Random//随机数 摇骰子都在这里
{
protected:
    QString seed;
public:
    Random(){}
    virtual ~Random(){}
    virtual void setseed(QString newseed) =0;
    QString getseed(){return seed;}
    virtual int randint(int min, int max) =0;
};


class SimpleRandom : public Random
{
private:
    qint64 now;
public:
    void setseed(QString newseed) override { seed = newseed; now = seed.toInt(); }
//    QString getseed() override { return seed; }
    int generate() {//随机生成一个0~32767的数字
        now = 214013 * now;
        now &= 0x7FFFFFFF;
        now = (now + 2531011);

        return (now >> 16) & 0x7FFF;
    }
    int randint(int min, int max) override//含上下界
    {
        int range = max - min + 1;
        int a = generate();
        while (a / range == 0x7FFF / range) { a = generate(); }
        return a % range + min;
    }
    SimpleRandom():SimpleRandom(QN(time(NULL))){}
    SimpleRandom(QString seedstr){ seed=seedstr; now = seed.toInt(); }//默认取时间
};

class mt19937Random : public Random
{
public:
    mt19937Random():mt19937Random(QN(time(NULL))){}
    mt19937Random(QString seedstr){ seed=seedstr;mt19937Random::setseed(seedstr); }
    void setseed(QString newseed) override {
        seed=newseed;
        std::string seedstr=seed.toStdString();
        std::seed_seq seed(seedstr.begin(), seedstr.end());
        mt=std::mt19937(seed);
    }
//        QString getseed() override{return seed;}
    int randint(int min, int max) override
    {
        int range = max-min+1;
        uint32_t a=mt();
        uint32_t nrep=uint32_t(4294967295)/range;
        while(a/range== nrep)a=mt();

        return a/nrep +min;
    }

private:
    std::mt19937 mt;
};

class RandomMaker
{
public:
    static void make_random(Random *&r, GT::RANDOMTYPE type,QString seed)
    {
        seed=seed.simplified();
        seed=seed.split(' ')[0];
        if(r)
        {delete r;r=NULL;}
        switch(type)
        {
        case GT::SIMPLERANDOM:
            r=new SimpleRandom(seed);
            return;
        case GT::MT19937RANDOM:
            r=new mt19937Random(seed);
            return;
        default:throw 0;
        }
    }

};

#endif // RANDOM_H
