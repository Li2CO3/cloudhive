#include "planebattlepage.h"

void PlaneBattlePage::load()
    {
    NEW_LABEL_MW(challenger,180,25,300,50,MW->name,40);
    NEW_LABEL_MW(Boss,930,25,300,50,PG->boss->name(),40);
    NEW_BUTTON_IMAGE_MW(challengericon,70,5,100,100,QString("res/icon/"+MW->icon));
    NEW_BUTTON_IMAGE_MW(Bossicon,820,5,100,100,PG->boss->icon());
    NEW_LABEL_ALIGN_MW(bulletin,100,810,1500,200,"",20,Top|Qt::AlignLeft);
    NEW_BUTTON_MW(ClearMark,600,50,100,50,"清空标记",20);


    NEW_LABEL_MW(turninfo,1100,810,400,30,"",25);
    NEW_LABEL_MW(remainheadinfo,1100,860,400,30,"",25);
    NEW_LABEL_MW(remainmissileinfo,1100,910,400,30,"",25);
    NEW_LABEL_MW(nukewarning,1100,960,400,30,"",25);
    ClearMark->connect(ClearMark,&QPushButton::clicked, PG,[=](){PG->Clear_marks();});

        for(int i=0;i<14;i++)for(int j=0;j<14;j++)
            {
                NEW_PLANEBUTTON_MW(playergrid[i][j],10+i*50,110+j*50,45,45,"",15,MW);

                NEW_PLANEBUTTON_MW(bossgrid[i][j],760+i*50,110+j*50,45,45,"",15,MW);

                playergrid[i][j]->PG=PG;
                bossgrid[i][j]->PG=PG;
                playergrid[i][j]->dual_button=bossgrid[i][j];
                bossgrid[i][j]->dual_button=playergrid[i][j];

                playergrid[i][j]->x=i, playergrid[i][j]->y=j; playergrid[i][j]->isbossgrid=false;
                bossgrid[i][j]->x=i, bossgrid[i][j]->y=j; bossgrid[i][j]->isbossgrid=true;

                //QPushButton::connect(playergrid[i][j],&QPushButton::clicked,MW,[=]()
                //                     {qDebug()<<playergrid[i][j]->x<<playergrid[i][j]->y;});
                //QMenu *menu = new QMenu(bossgrid[i][j]);

                //QAction *markUp   = new QAction ("上", menu);
                //QAction *markDown = new QAction ("下", menu);
                //QAction *markLeft = new QAction ("左", menu);
                //QAction *markRight= new QAction ("右", menu);
                //menu->addAction(markUp);
                //menu->addAction(markDown);
                //menu->addAction(markLeft);
                //menu->addAction(markRight);
                //bossgrid[i][j]->setMenu(menu);

            }


        PG->connect(PG,&PlaneBattleGame::signal_text,bulletin,[=](QString str)
                    {if(str=="")bulletin->setText("");
                    else {QString s=(bulletin->text()+"\n"+str);
                        auto t=s.split('\n');
                        if(t.length()>6)
                            s=s.mid(t[0].length()+1);
                        bulletin->setText(s);
                        }

                        });



        for(int i=0;i<14;i++)for(int j=0;j<14;j++){playergrid[i][j]->ready=true; bossgrid[i][j]->ready=true;}
        NEW_BUTTON_MW(back,10,10,50,50,"返回",20);
        NEW_BUTTON_MW(confirmput,470,50,100,50,"确认",20);
        PG->connect(confirmput,&QPushButton::clicked, PG,[=](){emit PG->signal_player_put_planes_done();});
        MW->connect(back,&QPushButton::clicked,MW,[=](){MW->load_page(STARTPAGE);});
        PG->connect(PG,&PlaneBattleGame::signal_update_grid,PG,[=](){update();});
        emit PG->signal_update_grid();
    }
    void PlaneBattlePage::update()
    {
        if(PG->n_playerplanes==3 && PG->game_status==PT::Put_Planes)confirmput->show(); else confirmput->hide();

        this->turninfo->setText("回合:" + QN(PG->round));
        this->remainheadinfo->setText("剩余要害:"+(PG->critical_rule==0?QN(PG->n_boss_remain()):"?")+'/'+QN(PG->boss->nPlane()));
        this->remainmissileinfo->setText("当前连发:"+QN(PG->n_missile_togo)+'/'+QN(PG->n_missile_this_turn));
        if(PG->boss_id!=2)nukewarning->hide();
        else
            {
            QString str="";
            Boss_Nuke *b = static_cast<Boss_Nuke*>(PG->boss);
            if(b->nuked)str="核弹:已发射";
            else {
                if(b->pNuke()>=60)str="核弹:"+QN(b->pNuke()/10)+"."+QN(b->pNuke()%10)+"%";
                else nukewarning->hide();
                }
            nukewarning->setText(str);
            }


    }


    QPixmap PlaneCellButton::cellpic(PT::Cell_Plane_type real_t,
                    PT::Cell_Strike_type strike_t,bool strike_new,
                    PT::Cell_Mark_type mark_t,
                    int pic_x, int pic_y, bool see_through)
    {
        QPixmap pic(pic_x, pic_y);
        QColor const colors[3][4]={{/*QColor(236,236,236)*/QColor(255,255,255),QColor(224,255,224),QColor(224,255,224),QColor(226,226,115)},//未打击
                                     {QColor(176,224,255),QColor(255,160,160),QColor(0,0,0),QColor(24,24,24)},//被打击
                                     {QColor(160,255,160),QColor(255,104,104),QColor(114,51,4),QColor(114,51,4)}};//被侦察。最后一个不存在
        QString const cellsymbol[3][4]={{" ","+","★","R"},{"o","⊕","★","114514"},{"-","+","114514","114514"}};
        //int const textsizes[2][3]={{7,7,7},{6,7,7}};
        QColor const textcolors[2][3]={{QColor(0,0,0),QColor(0,0,0),QColor(255,0,0)},{QColor(255,0,0),QColor(0,0,0),QColor(255,0,0)}};

        int show_type_id=real_t;
        if(!see_through)
        {
            if(PG->boss_grid[x][y].strike==PT::STRIKE_NO){show_type_id=0;}
            else
                show_type_id= ( (PG->critical_rule==PT::CRITICAL_SHOW ||//显示要害
                                 (PG->critical_rule==PT::CRITICAL_SHOW_FIRST && x==PG->critical_first_x && y==PG->critical_first_y) //或者首要害且这里是
                                 )?  int(real_t):(real_t==2?1:real_t));//则显示真实，否则要害2也显示机身1
        }
        pic.fill(colors[strike_t][show_type_id]);

        QPainter p(&pic);
        QFont f;
        f.setPixelSize((25));
        p.setFont(f);


        p.setPen(QPen(textcolors[0][show_type_id]));
        QString symbol=cellsymbol[(strike_t==1&&!strike_new )?0:strike_t][show_type_id];

        if(isbossgrid) for(int i=PG->nmarks-1;i>=0;i--)
            {
                if(PG->marks[i].centerx ==x && PG->marks[i].centery==y && symbol!="★")symbol="☆";
                if(PT::plane_occupies(PG->marks[i],x,y) && PG->boss_grid[x][y].strike==PT::STRIKE_NO) symbol="+";


            }


        int xx = (pic_x - p.fontMetrics().horizontalAdvance(symbol)) / 2;
        int yy = (pic_y + p.fontMetrics().height())/ 2-p.fontMetrics().descent();


        if(symbol=="-"){xx-=1;yy-=1;}
        if(symbol=="o"){yy-=4;}
        if(symbol=="⊕"){yy+=1;}
        p.drawText(xx, yy, symbol);

        if(hover)
        {
            int t=4;
            QPen pen; pen.setWidth(4);
            pen.setBrush(QBrush(QColor(233,233,0)));
            p.setPen(pen);
            p.drawRect(8,8,pic_x-16,pic_y-16);
//            pic.fill(Qt::yellow);
        }
        p.end();

        return pic;
    }

