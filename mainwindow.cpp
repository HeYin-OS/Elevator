#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QList>
#include <QString>
#include <QTimer>
#include <QDialog>
#include <QFont>
#include <string>
#include <random>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->timer=new QTimer(this);
    this->timer->setInterval(3000);
    this->timer->setSingleShot(false);

    connect(this->ui->a_e,&QAction::triggered,this,&QMainWindow::close);

    QList<QPushButton*> pblist=ui->f_b->findChildren<QPushButton*>();
    foreach (auto b, pblist)
    {
        char index=b->objectName().toStdString().at(3);
        if(b->objectName().contains("u"))
        {
            string temp="f_led_u";
            temp+=index;
            QString objname(QString::fromLocal8Bit(temp.data()));
            connect(b,&QPushButton::pressed,this,[=](){
                if(this->buttoncolorbox[index-'1'][0]==MainWindow::COLOR_RED)
                {
                    if(index!='1')
                    {
                        if(this->buttoncolorbox[index-'1'][1]==MainWindow::COLOR_RED)
                        {
                            ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                            this->buttoncolorbox[index-'1'][0]=MainWindow::COLOR_GREEN;
                            if(!(this->TIMER_IS_ON))
                            {
                                this->timer->start();
                                this->TIMER_IS_ON=true;
                            }
                            if(!(push_into_queue(index-'0')))
                            {
                                this->buttoncolorbox[index-'1'][0]=MainWindow::COLOR_RED;
                            }
                            else
                            {
                                this->buttonpressed[index-'0']=1;
                            }
                        }
                        else
                        {
                            ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                            this->buttoncolorbox[index-'1'][0]=MainWindow::COLOR_GREEN;
                        }
                    }
                    else
                    {
                        ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                        this->buttoncolorbox[index-'1'][0]=MainWindow::COLOR_GREEN;
                        if(!(this->TIMER_IS_ON))
                        {
                            this->timer->start();
                            this->TIMER_IS_ON=true;
                        }
                        if(!(push_into_queue(index-'0')))
                        {
                            this->buttoncolorbox[index-'1'][0]=MainWindow::COLOR_RED;
                        }
                        else
                        {
                            this->buttonpressed[index-'0']=1;
                        }
                    }
                }
            });
        }
        else if(b->objectName().contains("d"))
        {
            string temp="f_led_d";
            temp+=index;
            QString objname(QString::fromLocal8Bit(temp.data()));
            connect(b,&QPushButton::pressed,this,[=](){
                if(this->buttoncolorbox[index-'1'][1]==MainWindow::COLOR_RED)
                {
                    if(index!='7')
                    {
                        if(this->buttoncolorbox[index-'1'][0]==MainWindow::COLOR_RED)
                        {
                            ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                            this->buttoncolorbox[index-'1'][1]=MainWindow::COLOR_GREEN;
                            this->buttonpressed[index-'0']=1;
                            if(!(this->TIMER_IS_ON))
                            {
                                this->timer->start();
                                this->TIMER_IS_ON=true;
                            }
                            if(!(push_into_queue(index-'0')))
                            {
                                this->buttoncolorbox[index-'1'][1]=MainWindow::COLOR_RED;
                                this->buttonpressed[index-'0']=0;
                            }
                        }
                        else
                        {
                            ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                            this->buttoncolorbox[index-'1'][1]=MainWindow::COLOR_GREEN;
                            this->buttonpressed[index-'0']=1;
                        }
                    }
                    else
                    {
                        ui->f_b->findChild<QFrame*>(objname)->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
                        this->buttoncolorbox[index-'1'][1]=MainWindow::COLOR_GREEN;
                        this->buttonpressed[index-'0']=1;
                        if(!(this->TIMER_IS_ON))
                        {
                            this->timer->start();
                            this->TIMER_IS_ON=true;
                        }
                        if(!(push_into_queue(index-'0')))
                        {
                            this->buttoncolorbox[index-'1'][1]=MainWindow::COLOR_RED;
                            this->buttonpressed[index-'0']=0;
                        }
                    }
                }
            });
        }
    }

    QList<QPushButton*> iblist=ui->f_ib->findChildren<QPushButton*>();
    foreach (auto b, iblist)
    {
        char index=b->objectName().toStdString().at(4);
        connect(b,&QPushButton::pressed,this,[=](){
            this->inner_button_switch(index-'0',1);
            this->buttonpressed[index-'0']=1;
            if(!(this->TIMER_IS_ON))
            {
                this->timer->start();
                this->TIMER_IS_ON=true;
            }
            if(!(push_into_queue(index-'0')))
            {
                this->buttonpressed[index-'0']=0;
                this->inner_button_switch(index-'0',0);
            }
        });
    }

    connect(this->timer,&QTimer::timeout,this,[=](){
        this->elevator_run();
    });
    connect(this->ui->b_c,&QPushButton::pressed,this,[=](){
        if(MainWindow::nowstatus==MainWindow::STATUS_STOP)
        {
            this->ui->screen->setText(QString::fromLocal8Bit(MainWindow::SCREEN_WORDS[7]));
        }
        else
        {
            QDialog* dilg=new QDialog(this);
            dilg->setAttribute(Qt::WA_DeleteOnClose,true);
            dilg->setWindowTitle("ElevCon :: 警告 [0001]");
            dilg->setFixedHeight(200);
            dilg->setFixedWidth(360);
            dilg->setWindowIcon(QIcon(":/new/prefix1/dilg_icon.png"));
            QFrame* img=new QFrame(dilg);
            img->setGeometry(20,50,100,100);

            srand(time(nullptr));
            int flag=rand()%4+1;
            switch(flag)
            {
                case 1:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_1.png);");break;}
                case 2:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_2.png);");break;}
                case 3:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_3.png);");break;}
                case 4:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_4.png);");break;}
            }

            QLabel* lb=new QLabel(dilg);
            lb->setText("电梯在运行时，无法切换至此模式。");
            lb->setStyleSheet("color:red");
            lb->setGeometry(130,50,200,20);
            QPushButton* ok=new QPushButton(dilg);
            ok->setText("OK");
            ok->setFont(QFont("等线",11,-1,false));
            ok->setGeometry(170,110,100,40);
            connect(ok,&QPushButton::pressed,dilg,&QDialog::close);
            dilg->show();
        }
    });
    connect(this->ui->b_f,&QPushButton::pressed,this,[=](){
        if(MainWindow::nowstatus==MainWindow::STATUS_STOP)
        {
            this->ui->screen->setText(QString::fromLocal8Bit(MainWindow::SCREEN_WORDS[8]));
        }
        else
        {
            QDialog* dilg=new QDialog(this);
            dilg->setAttribute(Qt::WA_DeleteOnClose,true);
            dilg->setWindowTitle("ElevCon :: 警告 [0002]");
            dilg->setFixedHeight(200);
            dilg->setFixedWidth(360);
            dilg->setWindowIcon(QIcon(":/new/prefix1/dilg_icon.png"));
            QFrame* img=new QFrame(dilg);
            img->setGeometry(20,50,100,100);
            srand(time(nullptr));
            int flag=rand()%4+1;
            switch(flag)
            {
                case 1:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_1.png);");break;}
                case 2:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_2.png);");break;}
                case 3:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_3.png);");break;}
                case 4:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_4.png);");break;}
            }
            QLabel* lb=new QLabel(dilg);
            lb->setText("电梯在运行时，无法切换至此模式。");
            lb->setStyleSheet("color:red");
            lb->setGeometry(130,50,200,20);
            QPushButton* ok=new QPushButton(dilg);
            ok->setText("OK");
            ok->setFont(QFont("等线",11,-1,false));
            ok->setGeometry(170,110,100,40);
            connect(ok,&QPushButton::pressed,dilg,&QDialog::close);
            dilg->show();
        }
    });
    connect(this->ui->b_e,&QPushButton::pressed,this,[=](){
        if(MainWindow::nowstatus==MainWindow::STATUS_STOP)
        {
            this->ui->screen->setText(QString::fromLocal8Bit(MainWindow::SCREEN_WORDS[9]));
        }
        else
        {
            QDialog* dilg=new QDialog(this);
            dilg->setAttribute(Qt::WA_DeleteOnClose,true);
            dilg->setWindowTitle("ElevCon :: 警告 [0003]");
            dilg->setFixedHeight(200);
            dilg->setFixedWidth(360);
            dilg->setWindowIcon(QIcon(":/new/prefix1/dilg_icon.png"));
            QFrame* img=new QFrame(dilg);
            img->setGeometry(20,50,100,100);
            srand(time(nullptr));
            int flag=rand()%4+1;
            switch(flag)
            {
                case 1:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_1.png);");break;}
                case 2:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_2.png);");break;}
                case 3:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_3.png);");break;}
                case 4:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_4.png);");break;}
            }
            QLabel* lb=new QLabel(dilg);
            lb->setText("电梯在运行时，无法切换至此模式。");
            lb->setStyleSheet("color:red");
            lb->setGeometry(130,50,200,20);
            QPushButton* ok=new QPushButton(dilg);
            ok->setText("OK");
            ok->setFont(QFont("等线",11,-1,false));
            ok->setGeometry(170,110,100,40);
            connect(ok,&QPushButton::pressed,dilg,&QDialog::close);
            dilg->show();
        }
    });
    connect(this->ui->b_n,&QPushButton::pressed,this,[=](){
        if(MainWindow::nowstatus==MainWindow::STATUS_STOP)
        {
            this->ui->screen->setText(QString::fromLocal8Bit(MainWindow::SCREEN_WORDS[this->nowfloor-1]));
        }
        else
        {
            QDialog* dilg=new QDialog(this);
            dilg->setAttribute(Qt::WA_DeleteOnClose,true);
            dilg->setWindowTitle("ElevCon :: 警告 [0004]");
            dilg->setFixedHeight(200);
            dilg->setFixedWidth(360);
            dilg->setWindowIcon(QIcon(":/new/prefix1/dilg_icon.png"));
            QFrame* img=new QFrame(dilg);
            img->setGeometry(20,50,100,100);
            srand(time(nullptr));
            int flag=rand()%4+1;
            switch(flag)
            {
                case 1:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_1.png);");break;}
                case 2:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_2.png);");break;}
                case 3:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_3.png);");break;}
                case 4:{img->setStyleSheet("background-image: url(:/new/prefix1/rand_emg_4.png);");break;}
            }
            QLabel* lb=new QLabel(dilg);
            lb->setText("电梯在运行时，无法切换至此模式。");
            lb->setStyleSheet("color:red");
            lb->setGeometry(130,50,200,20);
            QPushButton* ok=new QPushButton(dilg);
            ok->setText("OK");
            ok->setFont(QFont("等线",11,-1,false));
            ok->setGeometry(170,110,100,40);
            connect(ok,&QPushButton::pressed,dilg,&QDialog::close);
            dilg->show();
        }
    });
    connect(this->ui->a_i,&QAction::triggered,this,[=](){
        QDialog* dilg=new QDialog(this);
        dilg->setAttribute(Qt::WA_DeleteOnClose,true);
        dilg->setWindowTitle("ElevCon :: Information");
        dilg->setFixedHeight(320);
        dilg->setFixedWidth(480);
        dilg->setWindowIcon(QIcon(":/new/prefix1/information_icon.ico"));

        QFrame* img=new QFrame(dilg);
        img->setGeometry(30,100,100,100);
        img->setStyleSheet("background-image: url(:/new/prefix1/my_github_icon.png);");

        QFrame* l=new QFrame(dilg);
        l->setGeometry(135,100,5,100);
        l->setStyleSheet("background-image: url(:/new/prefix1/line.png);");

        QFrame* vl=new QFrame(dilg);
        vl->setGeometry(30,250,420,5);
        vl->setStyleSheet("background-image: url(:/new/prefix1/vline.png);");

        QFrame* git=new QFrame(dilg);
        git->setGeometry(145,100,100,100);
        git->setStyleSheet("background-image: url(:/new/prefix1/github.png);");

        QLabel* lb1=new QLabel(dilg);
        lb1->setText("本软件开源，请勿作商业用途。");
        lb1->setStyleSheet("color:red;");
        lb1->setGeometry(280,100,200,20);

        QLabel* lb2=new QLabel(dilg);
        lb2->setText("源码可从github下载。");
        lb2->setStyleSheet("color:purple;");
        lb2->setGeometry(280,120,200,20);

        QLabel* link=new QLabel(dilg);
        link->setOpenExternalLinks(true);
        link->setText("<style> a {text-decoration: none} </style> <a href=\"https://github.com/HeYin-OS/Elevator\">[前往]");
        link->setGeometry(280,140,200,20);

        QLabel* cr=new QLabel(dilg);
        cr->setText("版权：Github用户——HeYin-OS，软件版本：0.0.1");
        cr->setGeometry(100,255,480,20);

        QLabel* pcr=new QLabel(dilg);
        pcr->setText("平台版权：2020 The Qt Company，Qt Creator版本：4.14.2");
        pcr->setGeometry(72,275,480,20);

        QPushButton* ok=new QPushButton(dilg);
        ok->setText("OK");
        ok->setFont(QFont("等线",11,-1,false));
        ok->setGeometry(335,180,50,20);

        connect(ok,&QPushButton::pressed,dilg,&QDialog::close);

        dilg->show();
    });
}

void MainWindow::replace_screen(string s_w)
{
    ui->screen->setText(QString::fromLocal8Bit(s_w.data()));
}

bool MainWindow::push_into_queue(int x)
{
    if(this->nowstatus==MainWindow::STATUS_STOP)
    {
        if(x==this->nowfloor)
        {
            modify_button_led(x,MainWindow::COLOR_GREEN);
            this->timer->stop();
            this->TIMER_IS_ON=false;
            return false;
        }
        else if(x>this->nowfloor)
        {
            this->nowstatus=MainWindow::STATUS_UP;
            this->upqueue.push(x);
            return true;
        }
        else if(x<this->nowfloor)
        {
            this->nowstatus=MainWindow::STATUS_DOWN;
            this->downqueue.push(x);
            return true;
        }
    }
    else if(this->nowstatus==MainWindow::STATUS_UP)
    {
        if(x==this->nowfloor)
        {
            modify_button_led(x,MainWindow::COLOR_GREEN);
            this->timer->stop();
            this->TIMER_IS_ON=false;
            if(!(upqueue.empty()))
            {
                this->timer->start();
                this->TIMER_IS_ON=true;
            }
            return false;
        }
        else if(x>this->nowfloor)
        {
            this->upqueue.push(x);
            return true;
        }
        else if(x<this->nowfloor)
        {
            this->downqueue.push(x);
            return true;
        }
    }
    else if(this->nowstatus==MainWindow::STATUS_DOWN)
    {
        if(x==this->nowfloor)
        {
            modify_button_led(x,MainWindow::COLOR_GREEN);
            this->timer->stop();
            this->TIMER_IS_ON=false;
            return false;
            if(!(downqueue.empty()))
            {
                this->timer->start();
                this->TIMER_IS_ON=true;
            }
        }
        else if(x<this->nowfloor)
        {
            this->downqueue.push(x);
            return true;
        }
        else if(x>this->nowfloor)
        {
            this->upqueue.push(x);
            return true;
        }
    }
    return false;
}

void MainWindow::modify_button_led(int x,int color)
{
    if(color==MainWindow::COLOR_GREEN)
    {
        if(x!=7&&x!=1)
        {
            string temp1="f_led_u";
            string temp2="f_led_d";
            temp1+=(x+'0');
            temp2+=(x+'0');
            QString objname1(QString::fromLocal8Bit(temp1.data()));
            QString objname2(QString::fromLocal8Bit(temp2.data()));
            ui->f_b->findChild<QFrame*>(objname1)->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
            ui->f_b->findChild<QFrame*>(objname2)->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
            this->buttoncolorbox[x-1][0]=MainWindow::COLOR_RED;
            this->buttoncolorbox[x-1][1]=MainWindow::COLOR_RED;
        }
        else if(x==1)
        {
            string temp1="f_led_u";
            temp1+=(x+'0');
            QString objname1(QString::fromLocal8Bit(temp1.data()));
            ui->f_b->findChild<QFrame*>(objname1)->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
            this->buttoncolorbox[x-1][0]=MainWindow::COLOR_RED;
            this->buttoncolorbox[x-1][1]=MainWindow::COLOR_RED;
        }
        else if(x==7)
        {
            string temp2="f_led_d";
            temp2+=(x+'0');
            QString objname2(QString::fromLocal8Bit(temp2.data()));
            ui->f_b->findChild<QFrame*>(objname2)->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
            this->buttoncolorbox[x-1][0]=MainWindow::COLOR_RED;
            this->buttoncolorbox[x-1][1]=MainWindow::COLOR_RED;
        }
    }
}

void MainWindow::elevator_run()
{
    if(this->nowstatus==MainWindow::STATUS_UP)
    {
        if(!(this->upqueue.empty()))
        {
            if(upqueue.top()==nowfloor+1)
            {
                change_floor_led(MainWindow::STATUS_UP,this->nowfloor);
                this->buttonpressed[nowfloor]=0;
                this->nowfloor++;
                this->inner_button_switch(nowfloor,0);
                replace_screen(this->SCREEN_WORDS[this->nowfloor-1]);
                modify_button_led(this->nowfloor,MainWindow::COLOR_GREEN);
                int top=upqueue.top();
                while(true)
                {
                    upqueue.pop();
                    if(upqueue.empty())
                    {
                        break;
                    }
                    if(upqueue.top()!=top)
                    {
                        break;
                    }
                }
                if(upqueue.empty())
                {
                    this->nowstatus=MainWindow::STATUS_STOP;
                    this->timer->stop();
                    this->TIMER_IS_ON=false;
                    if(!(downqueue.empty()))
                    {
                        this->nowstatus=MainWindow::STATUS_DOWN;
                        this->timer->start();
                        this->TIMER_IS_ON=true;
                    }
                }
            }
            else
            {
                change_floor_led(MainWindow::STATUS_UP,this->nowfloor);
                this->buttonpressed[nowfloor]=0;
                this->nowfloor++;
                this->inner_button_switch(nowfloor,0);
                replace_screen(this->SCREEN_WORDS[this->nowfloor-1]);
                modify_button_led(this->nowfloor,MainWindow::COLOR_GREEN);
            }
        }
    }
    else if(this->nowstatus==MainWindow::STATUS_DOWN)
    {
        if(!(this->downqueue.empty()))
        {
            if(downqueue.top()==nowfloor-1)
            {
                change_floor_led(MainWindow::STATUS_DOWN,this->nowfloor);
                this->buttonpressed[nowfloor]=0;
                this->nowfloor--;
                this->inner_button_switch(nowfloor,0);
                replace_screen(this->SCREEN_WORDS[this->nowfloor-1]);
                modify_button_led(this->nowfloor,MainWindow::COLOR_GREEN);
                int top=downqueue.top();
                while(true)
                {
                    downqueue.pop();
                    if(downqueue.empty())
                    {
                        break;
                    }
                    if(downqueue.top()!=top)
                    {
                        break;
                    }
                }
                if(downqueue.empty())
                {
                    this->nowstatus=MainWindow::STATUS_STOP;
                    this->timer->stop();
                    this->TIMER_IS_ON=false;
                    if(!(upqueue.empty()))
                    {
                        this->nowstatus=MainWindow::STATUS_UP;
                        this->timer->start();
                        this->TIMER_IS_ON=true;
                    }
                }
            }
            else
            {
                change_floor_led(MainWindow::STATUS_DOWN,this->nowfloor);
                this->buttonpressed[nowfloor]=0;
                this->nowfloor--;
                this->inner_button_switch(nowfloor,0);
                replace_screen(this->SCREEN_WORDS[this->nowfloor-1]);
                modify_button_led(this->nowfloor,MainWindow::COLOR_GREEN);
            }
        }
    }
}

void MainWindow::change_floor_led(int dirction,int origin_floor)
{
    if(dirction==MainWindow::STATUS_UP)
    {
        string temp1="s_led_";
        string temp2="s_led_";
        temp1+=origin_floor+'0';
        temp2+=(origin_floor+1+'0');
        ui->f_s->findChild<QFrame*>(QString::fromLocal8Bit((temp1.data())))->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
        ui->f_s->findChild<QFrame*>(QString::fromLocal8Bit((temp2.data())))->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
    }
    else if(dirction==MainWindow::STATUS_DOWN)
    {
        string temp1="s_led_";
        string temp2="s_led_";
        temp1+=origin_floor+'0';
        temp2+=(origin_floor-1+'0');
        ui->f_s->findChild<QFrame*>(QString::fromLocal8Bit((temp1.data())))->setStyleSheet("background-image: url(:/new/prefix1/red.png);");
        ui->f_s->findChild<QFrame*>(QString::fromLocal8Bit((temp2.data())))->setStyleSheet("background-image: url(:/new/prefix1/green.png);");
    }
}

void MainWindow::inner_button_switch(int bnum,int towhat)
{
    string temp="b_i_";
    temp+=(bnum+'0');
    if(towhat==0)
    {
        this->ui->f_ib->findChild<QPushButton*>(QString::fromLocal8Bit(temp.data()))->setStyleSheet("");
    }
    else if(towhat==1)
    {
        this->ui->f_ib->findChild<QPushButton*>(QString::fromLocal8Bit(temp.data()))->setStyleSheet("border:2px solid green;");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
