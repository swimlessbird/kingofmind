#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myhead.h"
#include "mythread.h"
#include <QMutex>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void check_lstMatch();          //检查匹配表
    void send_matchUser_toThread(); //发送对手信息给线程
    void get_Topic();               //从数据库中获取题目
    void Mysql_Init();              //初始化数据库连接
     mythread * search_thread(QString);      //根据用户名寻找thread号

private:
    Ui::Widget *ui;

    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;

    QSqlDatabase db;

    QMap<QTcpSocket *, QString> lstMatch;       //匹配中用户列表,绑定套接字与用户名
    QMap<QTcpSocket *, mythread *> lstThread;   //绑定线程与套接字
    QMap<QTcpSocket *, QString> lstOnline;      //在线用户列表，绑定套接字与用户名

    QList<Exam_Info> lstExam;                   //题目列表

    QMutex mutex;

public slots:
    void acceptClient();                                //接受客户端连接
    void deal_thread_result(char *, char *);            //处理结果显示在ui界面
    void deal_thread_match(QTcpSocket *, char *);       //匹配
    void deal_user_login(QTcpSocket *, char *);         //用户登录
    void deal_thread_cancel(QTcpSocket *, char *);      //用户取消匹配
    void deal_thread_disConnect(QTcpSocket *);          //用户断开连接
    void deal_thread_exitLogin(QTcpSocket *, char *);   //用户退出登录

    void tran_score(int, QString);          //转发分数
    void tran_giveup(QString);

private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
