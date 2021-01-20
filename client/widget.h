#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myhead.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void Mysql_init();  //初始化数据库连接
    void Net_init();    //初始化网络连接
    void Timer_init();  //初始化定时器
    void ExamLoad();    //加载题库
    void Deal_Result(Result_msg tmp);    //处理返回的结果

private:
    Ui::Widget *ui;

    QSqlDatabase db;        //数据库操作句柄

    QTimer *timer;       //定时器

    QTcpSocket *Tsocket;     //网络通信套接字

    QList<Exam_Info> lstExam;   //存放题目的list表指针
    Exam_Info *exam_content;    //存放题目内容的指针

    Cmd_Info buffer;        //存放命令信息

    /*窗口对象*/
    init_interface *init_obj;
    login_interface *log_obj;
    reg_interface *reg_obj;
    alonegame_interface *alg_obj;
    online_interface *onl_obj;
    battlegame_interface *btg_obj;
    /*----*/

public slots:

    /*处理来自Init界面的信号*/
    void deal_fromInit_toAlone();
    void deal_fromInit_toBattle();
    void deal_fromInit_close();

    /*处理来自AloneGame界面的信号*/
    void deal_fromAlo_toBack();
    void deal_fromAlo_toGame();
    void deal_count_over();

    /*处理来自Login界面的信号*/
    void deal_fromLogin_toBack();
    void deal_fromLogin_tolog();
    void deal_fromLogin_toreg();

    /*处理来自Reg界面的信号*/
    void deal_fromReg_toBack();
    void deal_fromReg_toReg();

    /*处理来自Online界面的信号*/
    void deal_fromOnl_toBack();
    void deal_fromOnl_toMatch();
    void deal_fromOnl_toCancel();

    /*处理来自BattleGame界面的信号*/
    void deal_fromBtg_toGiveUp();
    void deal_fromBtg_Over();
    void deal_fromBtg_getExam();

    void deal_fromBtg_Score();   //---------------------------

    /*处理通信套接字发出的信号*/
    void Read_Data();       //读取服务器发来的消息
    void Send_Data();       //向服务器发送数据
};

#endif // WIDGET_H
