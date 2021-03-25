#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <queue>
#include <string>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int COLOR_RED=0;
    static const int COLOR_GREEN=1;
    static const int STATUS_STOP=70;
    static const int STATUS_UP=71;
    static const int STATUS_DOWN=72;
    const string SCREEN_WORDS[10]={"F2","F1","01","02","03","04","05","CL","FX","EM"};

    int nowfloor=3;
    int nowstatus=STATUS_STOP;
    bool TIMER_IS_ON=false;
    int buttoncolorbox[7][2]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}};
    int buttonpressed[8]={0,0,0,0,0,0,0,0};

    priority_queue<int,vector<int>,greater<int>> upqueue;
    priority_queue<int> downqueue;

    QTimer* timer;

    MainWindow(QWidget *parent = nullptr);

    void replace_screen(string s_w);
    bool push_into_queue(int x);
    void modify_button_led(int x,int color);
    void elevator_run();
    void change_floor_led(int dirction,int origin_floor);
    void inner_button_switch(int bnum,int towhat);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
