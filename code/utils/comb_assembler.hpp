#include "../grid.h"
#include "../piece.h"
#include "mainwindow.h"
#include "modes/numcomb.h"
#ifndef COMB_ASSEMBLER_HPP
#define COMB_ASSEMBLER_HPP

#include <vector>
#include <queue>
#include <cstring>
#include "cardpool.h"
#include "qlabel.h"
#include "random.hpp"
#include <QDialog>
#include <QPushButton>
#include <QClipboard>
struct Assembling_result
{
    Piece pieces[20];
    int places[20];
    int point;
    bool useseed;

};
struct PieceSet
{
    bool isvalid=true;
    bool useseed;
    Piece pieces[20];
    PieceSet(){isvalid=false;}
    PieceSet(const Piece * const p){for(int i=0;i<20;i++) pieces[i]=p[i]; useseed=false;}
    PieceSet(CardPool *pool_shuffled)
        {for(int i=0;i<20;i++)pieces[i]=pool_shuffled->pieces[i];}
    PieceSet(bool u,QString string_of_seed_or_cards)//第一个空格前是seed或card可用。默认为seed。
        {
        QStringList lst = string_of_seed_or_cards.split(' ');

            if(u)
                {
                useseed=true;
                if(string_of_seed_or_cards=="")
                    {*this = PieceSet();return;}
                CardPool pool(GT::POOL_Normal);
                mt19937Random random(lst[0]);

                NumcombGame::Setup_Numcomb_Pool(&pool,&random);
                *this=PieceSet(&pool);
                }
            else
            {
                useseed=false;
                if(lst.length()<20){*this=PieceSet();return;}

                for(int i=0;i<20;i++) pieces[i]=lst[i];

            }
        }

};

class Comb_Assembler //:public QThread
{
public:
    /*    Q_OBJECT
    private:
        PieceSet p;
        Assembling_result *resultpl;
    public:
        Comb_Assembler(PieceSet pp, Assembling_result *rr){p=pp;resultpl=rr;}


        void run() override{*resultpl = assemble(p);}

    */

     static Assembling_result assemble(bool useseed, QString info)
    {

            return assemble(PieceSet(useseed, info), info);
        }
    static Assembling_result assemble(PieceSet p, QString info )
        {
      /*      Assembling_result rr;resultpl=rr;
            Comb_Assembler ca(p,rr);

    }

        Assembling_result foo(PieceSet p)
    */    if(!p.isvalid)return Assembling_result();
        int out[20];


        int pointscount[13]={0,1,2,3,4,5,6,7,8,9,10,10,10};
    //    int linelength[5]={3,4,5,4,3};
/*
        std::string const list[32] = {
                                      "....." ,


                                      "o...." , "....o" ,
                                      ".o..." , "...o." ,
                                      "..o.." ,
                                      "o...o" ,
                                      "oo..." , "o..o." , ".o..o" , "...oo" ,
                                      "o.o.." , "..o.o" , ".o.o." ,
                                      ".oo.." , "..oo." ,
                                      "oo..o" , "o..oo" ,
                                      "oo.o." , ".o.oo" , "o.o.o" ,
                                      "ooo.." , "o.oo." , ".oo.o" , "..ooo" ,
                                      ".ooo." ,
                                      "oo.oo" ,
                                      "o.ooo" , "ooo.o" ,
                                      ".oooo" , "oooo." ,


                                      "ooooo"};
*/
        //由于有一些内容是涉及0号块选择的，就统一放这里了。反正const的东西乘以20也不是大问题(嗯？？？已经把重复块只看一遍了。别的先不管了
        //通过对称性可以减少计算，但目前未制作。bool is_symmetry_kept [32]={1, 1,0, 1,0, 1, 1, 1,1,0,0, 1,0,1, 1,0, 1,0,1, 1,1,0,0, 1, 1, 1,0, 1,0, 1};
        int const linescount[32]={0,3,3,4,4,5,6,7,7,7,7,8,8,8,9,9,10,10,11,11,11,12,12,12,12,13,14,15,15,16,16,19};//遍历单数
        int const startbycount[20]={0,0,0,2,4,5,6,10,13,15,17,20,24,25,26,28,30,30,30,31};//遍历单数结构时从第几个往回跑
        int const larger_sameline[13]={0,5,6,4,8,9,7,10,12,11,0,0,0};//同方向的更大的数
        int const smaller_sameline[13]={0,0,0,0,3,1,2,6,4,5,7,9,8};//同方向的最小的数
        int const listid[32]={0,16,1,8,2,4, 17,24,18,9,3, 20,5,10, 12,6, 25,19, 26,11,21, 28,22,13,7, 14,27,23,29,15,30,31};
                    //连线情况按2进制来表示的值
            //这些二进制量是对应的。第一条线是123、148、81317。在这里由于从0开始计数，它们全都要减1。
        int const ldata1[19]={1,1,1,2,2,2,2,4,4,4,4,4,8,8,8,8,16,16,16},
                  ldata2[19]={1,2,4,1,2,4,8,1,2,4,8,16,2,4,8,16,4,8,16},
                  ldata3[19]={4,8,16,2,4,8,16,1,2,4,8,16,1,2,4,8,1,2,4};

        int placetable[13][19];//所在位置对应线的值。
        for(int i=0;i<13;i++){switch(i)
            {
            case 1:case 5:case 9:case 11: {for(int j=0;j<19;j++)placetable[i][j]=ldata1[j];break;}
            case 2:case 6:case 7:case 10: {for(int j=0;j<19;j++)placetable[i][j]=ldata2[j];break;}
            case 3:case 4:case 8:case 12: {for(int j=0;j<19;j++)placetable[i][j]=ldata3[j];break;}
            }
        };

        int best=0;//保存最佳数据。todo：同时保存放块方式

        for(int zeroth=0;zeroth<=19;zeroth++)//0号是第几块
        {
            {bool toskip=false;
            for(int i=0;i<zeroth;i++)
                    if(p.pieces[i].to_string()==p.pieces[zeroth].to_string())
                {toskip=true;break;}
            if(toskip)continue;
             }

            Piece pieces[19];
            for(int i=0;i<19;i++) pieces[i]=p.pieces[i];
            if(zeroth<19) pieces[zeroth]=p.pieces[19];

            int bigtable[19][13];//打个大表：x号块在数字y上的方向是几。
            for(int x=0;x<19;x++)
                for(int y=1;y<13;y++)
                {
                    bigtable[x][y]=pieces[x].ValueofLine((y>=10)?16-y:y);//10对7 11对9 12对8，和654是一样的
                }
            int zerothpt=p.pieces[zeroth].sum();//0号位的得分。咱有算力，不差这20倍
            {



                int ncount[13]={0};//每种数字有几个
                    //12 8 4 3
                    //11 9 5 1
                    //10 7 6 2

                    for(int id=0;id<19;id++)//计算每种数字有几个
                {
                    if(pieces[id].x159()==10)ncount[11]++;
                    else ncount[pieces[id].x159()]++;
                    if(pieces[id].x348()==10)ncount[12]++;
                    else ncount[pieces[id].x348()]++;
                    ncount[pieces[id].x267()]++;

                }


                    int trial[13];//1~12号的尝试

                auto noconflict = [&] (int next){//不和更大数字的线重叠
                        int x=listid[trial[next]];//next占的线
                        while(next!=0)
                        {next=larger_sameline[next];
                            if(listid[trial[next]]&x) return false;
                        }
                        return true;
                    };

                    bool chart[14][19][19];//固定数字x以及更大的之后，每个块是否可放
                for(int y=0;y<19;y++)for(int z=0;z<19;z++) chart[13][y][z]=true;//1~12都会先赋值

                auto iscompatible = [&](int next)
                    {

                        for(int i=0;i<19;i++)//i位
                            for(int j=0;j<19;j++)//j块
                            {
                                chart[next][i][j]=(chart[next+1][i][j] &&//看上一个数的时候还没问题
                     ((bigtable[j][next]==10 || bigtable[j][next]==pointscount[next])//要么这块不怕这个数字
                                    ||
                      ( (listid[trial[next]] & placetable[next][i])==0)  )//要么这个位置没连这个数字
                                                     );
                            }

                        return hopcroftKarp(chart[next],next,zeroth,out);//https://blog.csdn.net/weixin_66767924/article/details/131694114
                    };




                    auto remaininglz = [&] (int next)//还剩几个赖子.例如：判定2可以有几个的时候，是 (赖子)-(赖子线用的)-(7线用的)-(6线用的)。
                    {
                    int freelz= 0;
                    while(next<10)
                        {
                        freelz-=std::max(linescount[trial[next]]-ncount[next],0);
                        next=larger_sameline[next];
                    }
                    freelz=freelz+ncount[next]-linescount[trial[next]];
                    return freelz;
                    };
                auto possiblemax = [&](int next)//理论最大值。
                {
                    int pt=0;
                    for(int i=next;i<=12;i++)//已经连的直接算
                        pt+=pointscount[i]*linescount[trial[i]];
                    pt+=zerothpt;
                    for(int i=1;i<next;i++)//还没连的全都算
                        pt+=pointscount[i]*ncount[i];
                    for(int i=10;i<=12;i++)//多的赖子按剩下的同线最大数算
                    {int w=i;while(w>=next)
                            w=smaller_sameline[w];
                        if(w>0)
                        pt+=remaininglz(w)*w;
                        }
                    return pt;

                };


                int next=12;for(int i=0;i<12;i++)trial[i]=0;
                    trial[12]=startbycount[ncount[12]]+1;
                    while(next<=12)
                    {
                        trial[next]--;
                        if(trial[next]<0)
                        {trial[next]=0;next++;continue;}


                        //qDebug()<<next<<"="<<trial[next]<<list[trial[next]];

                        if(noconflict(next)
                            &&
                            (possiblemax(next)>best)
                            &&
                            iscompatible(next))
                        {//qDebug()<<"v";
                            if(next==1){

                                best=0;
                                for(int i=1;i<=12;i++)
                                {best+=pointscount[i]*linescount[trial[i]];
                                }
                                best+=zerothpt;

                            }
                            else{next--;
                                trial[next]=startbycount[ncount[next]+(next>=10?0:remaininglz(next))]+1;
                            continue;}
                        }
                        //qDebug()<<"x";


                        continue;

                    }

            }


        }//zeroth
        Assembling_result solution;
        for(int i=0;i<20;i++)solution.pieces[i]=p.pieces[i];
        for(int i=0;i<20;i++)solution.places[i]=out[i];
        solution.point=best;
        QString str="拼装：";
        for(int i=0;i<20;i++)str=str+QN(solution.places[i])+' '; //qDebug()<<str<<",得分："<<solution.point;
        return solution;

    }


    static bool hopcroftKarp(bool data[19][19], int next, int zeroth, int *out=NULL) {


        bool vis[19]={false};
        int result[19];


        std::function<int(int)> check=[&](int i){//判定匹配。
    for(int j=0;j<19;j++){//让左边第i个点和右边第j个点尝试匹配。
        if(data[i][j]&&!vis[j]){
                    vis[j]=1;
                    if((result[j]==-1)||check(result[j])){
                        result[j]=i;
                        return 1;
                    }
                }
            }
            return 0;
        };
        auto g=[&](){int ans=0;
            for(int i=0;i<19;i++)result[i]=-1;
            for(int i=0;i<19;i++){
                memset(vis,0,sizeof(vis));
                if(check(i))ans++;//匹配成功
        }
            return ans;
        };
        //assert(g()==g());
        //if(g()==19 && next==1)
        //{
            //QString str="";
            //for(int i=0;i<19;i++) {str="";for(int j=0;j<19;j++) {;{str+=(data[i][j]?'o':'.');}if(j==18)//qDebug()<<str<<',';}}}
        int x=g();
        if(x==19 && next==1)
            if(out!=NULL)
                {
                    QString str="";
                    for(int i=0;i<19;i++)
                        {out[i]=result[i]+1;str+=QN(out[i]);}
                    //qDebug()<<str;

                    if(zeroth<19){
                        out[19]=out[zeroth];
                        out[zeroth]=0;
                    }
                    else {out[19]=0;}
                }
        return x==19;
    }
    static void Make_Assemble_Page(bool useseed, QString info, QDialog *dialog)
    {

        auto result=Comb_Assembler::assemble(useseed, info);

        dialog->setFixedSize(650,650);
        QLabel *text;
        QStringList lst = info.simplified().split(' ');
        if(lst.length()==0)return;

        QString title="拼装毁灭者 ";
        QString theseed="\n种子:";
        QFont f("consolas");
        f.setPixelSize(16); //label将要使用的字体
        if(useseed){theseed+=lst[0]; title+="种子: "+lst[0];
            QString str=theseed;
                switch(1)
                    {default:
                        case 1:
                    if ( str.isEmpty()) {
                        break;
                    }
                    // 创建一个QFontMetrics对象
                    QFontMetrics metrics(f);
                    // 初始化当前行的填充位置
                    int currentPlace = 0;
                    // 初始化结果字符串
                    QString result;
                    // 遍历每个字符
                    int placeofdots =-1, xofdots=-1;
                    int firstreturn = 0;
                    int const width1=550;
                    for (;firstreturn < str.length(); ++firstreturn) {
                        // 获取当前字符
                        const QChar &c = str[firstreturn];
                        // 计算当前字符的宽度
                        currentPlace +=metrics.horizontalAdvance(c);

                        if(placeofdots==-1 && currentPlace >=400) { currentPlace-=metrics.horizontalAdvance(c);placeofdots=firstreturn; xofdots = currentPlace;}

                        if(currentPlace >=width1)break;//第一行结束
                        }
                    if(firstreturn ==str.length()) {break;}//不变
                    //在[firstreturn]前换行。
                        int secondbegin = str.length();//[从secondbegin]开始显示
                        int remainingwidth = width1 - xofdots -metrics.horizontalAdvance("...  ");

                        for(;secondbegin>firstreturn; --secondbegin)
                        {
                            const QChar &c = str[secondbegin-1];
                            remainingwidth -=metrics.horizontalAdvance(c);
                            if(remainingwidth <0) break;
                        }
                        theseed =str.left(placeofdots)+" ... "+str.right(str.length()-secondbegin);

                    // 返回处理后的文本
                      }//switch
            }//useseed
        else {theseed="";title +="自选";}
        dialog->setWindowTitle(title);

        QString textstr = QN(result.point)+' '+theseed+'\n';
        for(int i=0;i<20;i++){textstr+=QN(result.places[i])+' '; }
        textstr+='\n';
        for(int i=0;i<20;i++){textstr+=result.pieces[i].to_string()+' '; if(i==9) textstr+='\n';}
        NEW_LABEL(text,50,12,550,100,textstr,12,dialog);

        text->setFont(f);
        text->setTextInteractionFlags(Qt::TextSelectableByMouse);

        QLabel *grid;
        Grid g;
        for(int i=0;i<20;i++) g.pieces[result.places[i]]=result.pieces[i];
        QPixmap map;g.Make_image(map,GT::STANDARD_OPEN);
        NEW_LABEL_IMAGE(grid,101,120,448,448,map,dialog);
        dialog->show();
        dialog->connect(dialog,&QDialog::rejected,dialog,&QDialog::deleteLater);



        QString command_big=useseed?"board":"boardc";
        QString command_small=useseed?"comb":"card";

        QPushButton *CopySmall;
        NEW_BUTTON(CopySmall,245,590,160,40, "复制"+command_small ,20,dialog);

        QPushButton *CopyBig;

        NEW_BUTTON(CopyBig,55,590,160,40, "复制"+command_big ,20,dialog);

        QPushButton *Close;
        NEW_BUTTON(Close,435,590,160,40,"返回",20,dialog);


        CopySmall->connect(CopySmall,&QPushButton::clicked,dialog,[=](){
            QApplication::clipboard()->setText( useseed? Make_Comb_Text(info,result) : Make_Card_Text(result));
            CopySmall->setText(command_small+"已复制!");

            CopyBig->setText("复制"+command_big);

        });
        CopyBig->connect(CopyBig,&QPushButton::clicked,dialog,[=](){
                        QApplication::clipboard()->setText(  useseed ? Make_Board_Text(info,result) : Make_BoardC_Text(result)  );
            CopySmall->setText("复制"+command_small);
            CopyBig->setText(command_big+"已复制!");
        });
        Close->connect(Close, &QPushButton::clicked, dialog, [=](){dialog->close();});
}

    //template <typename W>//
   /* QString inline show_string_in_rows(QLabel *textwidget, int nlines, QString text)
    {QLabel *label = textwidget;
        QFont font = label->font(); // 获取当前字体
        QFontMetrics metrics(font); // 获取字体度量
        QString textToAppend = "Thisisalongsentencethatwillbebrokenintolineslineslinesandlinesandwhateverandfloccinaucinihilipilification.";
        int currentWidth = 0;
        for (int i = 0; i < textToAppend.length(); ++i) {
                             if (i > 0 && currentWidth + metrics.horizontalAdvance(textToAppend[i]) > label->width()) {
                    // 如果当前字符加上之前的字符宽度超过标签宽度，添加换行符
                    label->setText(label->text() + "\n" + textToAppend[i]);
                    currentWidth = metrics.horizontalAdvance(textToAppend[i]);
                             } else {
                    // 否则，直接添加字符
                    label->setText(label->text() + textToAppend[i]);
                    currentWidth += metrics.horizontalAdvance(textToAppend[i]);
                             }
        }
        label->adjustSize();
    }

*/
///Paste: comb card board boardc
private:
static QString seed_text(QString seed){return "seed: "+seed;};
static QString place_text(Assembling_result r)
{QString str="score: "+QN(r.point)+'\n';
        for(int i=0;i<20;i++)
                str=str+QN(r.places[i])+' ';
        return str;}

static QString card_text(Assembling_result r)
{QString str="cards:\n";
        for(int i=0;i<20;i++)
                str=str+QString(r.pieces[i])+' '+(i%5==4?"\n":"");
        return str;}

static QString board_text(Assembling_result r)
{
        QString str, ind[11][5];
        for(int y=0;y<11;y++)for(int x=0;x<5;x++)
                {ind[y][x]="----";}
        for(int piecetime=0;piecetime<=19;piecetime++)
        {
                int pos=r.places[piecetime];
                ind[GT::GRID_Y[pos]-1][GT::GRID_X[pos]-1]=
                    r.pieces[piecetime].to_string()+' ';
        }
        for(int y=0;y<11;y++)for(int x=0;x<5;x++)
                {
                    str+=ind[y][x];
                    if(x!=4) str+='|';
                    if(x==4 && y!=11-1) str+='\n';
                }
        return str;

}
public:

static QString Make_Comb_Text(QString seed, Assembling_result result)
{
        return seed_text(seed)+'\n'+place_text(result);

}
static QString Make_Board_Text(QString seed, Assembling_result result)
{
        return seed_text(seed)+'\n'+place_text(result)+'\n'+board_text(result);
}
static QString Make_BoardC_Text(Assembling_result result)
{
        return place_text(result)+'\n'+card_text(result)+board_text(result);
}
static QString Make_Card_Text(Assembling_result result)
{
        return place_text(result)+'\n'+card_text(result);
}
///end comb board card boardc



};
#endif
