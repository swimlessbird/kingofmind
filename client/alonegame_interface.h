#ifndef ALONEGAME_INTERFACE_H
#define ALONEGAME_INTERFACE_H

#include <QWidget>
#include "struct_head.h"

namespace Ui {
class alonegame_interface;
}

class alonegame_interface : public QWidget
{
    Q_OBJECT

public:
    explicit alonegame_interface(QWidget *parent = 0);
    ~alonegame_interface();

    void get_list(QList<Exam_Info> lstExam);    //获取题目

    char show_test();                   //显示题目
    void answercheck(char answer);     //检测答案是否正确

    void setButton_Enable_false();      //设置按钮状态
    void setButton_Enable_true();
    void reset_radio();

private slots:
    void on_pushButton_alone_exit_clicked();    //退出单人答题页面

    void on_pushButton_begin_clicked();         //开始单人答题

    void on_pushButton_clicked();               //下一题

public slots:
    void deal_time_out();

signals:
    void alone_to_back();
    void alone_begin_game();
    void count_over();

private:
    Ui::alonegame_interface *ui;

    static int second;      //答题时限
    static int count;       //记录已做的题目数
    static int score;       //记录得分
    static int ans_ch;      //记录上一题的答案

    QList<Exam_Info> lstExam;       //存储题目的容器
};

#endif // ALONEGAME_INTERFACE_H
