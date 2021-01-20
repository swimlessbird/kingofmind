#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include "myhead.h"

class mythread : public QThread
{
    Q_OBJECT
public:
    explicit mythread(QTcpSocket *client_socket, QThread *parent = 0);

    void run();

    void MySql_Init_Open();      //初始化数据库

    void get_matchUser_info(QString);  //获取对手用户名
    void get_matchUser_score(int);      //获取对手分数
    void get_lstExam(QList<Exam_Info>);    //获取题目
    void send_exam();               //发送题目给客户端

    void send_score_fun(int);   //发送分数给主线程
    void send_giveUp_fun();       //发送放弃信号给主线程

    void send_giveUp_toClient();

signals:   
    void send_user_result(char *, char *);          //返回命令处理结果
    void send_user_match(QTcpSocket *, char *);     //返回寻求匹配的用户
    void send_user_login(QTcpSocket *, char *);     //返回已登录用户
    void send_user_cancel(QTcpSocket *, char *);    //返回取消匹配的用户
    void send_disConnect_socket(QTcpSocket *);      //返回断开连接的套接字
    void send_user_exitLogin(QTcpSocket *, char *);    //返回取消匹配的用户

    void send_score(int, QString);      //返回分数以及对手
    void send_give_up(QString);

private:
    QTcpSocket *client_socket;  //客户端socket
    QSqlDatabase db;            //数据库句柄

    QString against_username;   //对手用户名

    QList<Exam_Info> lstExam;   //获取的题目

public slots:
    void deal_readData();   //解析数据
    void deal_reg(char *username, char *password);
    void deal_log(char *username, char *password);
    void deal_exit(char *username);
    void deal_match(char *username);
    void deal_cancel_match(char *username);
    void deal_client_disConnect();
};

#endif // MYTHREAD_H
