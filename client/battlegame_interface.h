#ifndef BATTLEGAME_INTERFACE_H
#define BATTLEGAME_INTERFACE_H

//#include <QWidget>
#include "myhead.h"
#include <QTimer>

namespace Ui {
class battlegame_interface;
}

class battlegame_interface : public QWidget
{
    Q_OBJECT

public:
    explicit battlegame_interface(QWidget *parent = 0);
    ~battlegame_interface();

    void get_against_name(char *username);  //获取对手用户名
    void get_against_socre(int score);      //获取对手分数
    void start_timer();                 //启动定时器
    void deal_time_out();               //处理定时器响应
    void reset();                   //重置控件及变量
    void reset_radio();             //重置选项按钮
    void get_exam(Exam_Info);        //获取题目
    void show_exam();               //显示题目
    void check_answer(char answer);        //检查答案

    void set_radioEnable_true();
    void set_radioEnable_false();

    int ret_socre();

signals:
    void btg_to_giveup();
    void btg_to_Over();
    void btg_get_exam();

    void btg_send_score();

private slots:
    void on_pushButton_surrender_clicked();

private:
    Ui::battlegame_interface *ui;

    Exam_Info tmp_exam;

    QTimer *timer;
    static int second;
    static int m_score;     //自己的分数
    static int count;       //题目条数

    int a_score;        //对手的分数

    char answer;        //用于保存上一题的答案
};

#endif // BATTLEGAME_INTERFACE_H
