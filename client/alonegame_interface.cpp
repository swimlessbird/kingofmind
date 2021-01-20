#include "alonegame_interface.h"
#include "ui_alonegame_interface.h"
#include "widget.h"
#include <QMessageBox>

#define SECOND 15       //每一道题的答题时间
#define MAX_COUNT 10    //最大题目数量

int alonegame_interface::second = SECOND - 1;
int alonegame_interface::count = 1;
int alonegame_interface::score = 0;
int alonegame_interface::ans_ch = '\0';

alonegame_interface::alonegame_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::alonegame_interface)
{
    ui->setupUi(this);
    setWindowTitle("单人模式");

    //lcd显示初值
    ui->lcdNumber->display(SECOND);

    //按钮复位
    setButton_Enable_false();
}

alonegame_interface::~alonegame_interface()
{
    delete ui;
}

void alonegame_interface::on_pushButton_alone_exit_clicked()
{
    ui->pushButton_begin->setEnabled(true);
    ui->pushButton_begin->setText("开始答题");
    ui->pushButton->setText("下一题");

    //点击退出按钮，全局变量全部赋为初始值
    second = SECOND;
    ui->lcdNumber->display(second);
    count = 1;
    score = 0;
    QString str = QString("%1分!").arg(score);
    ui->label_score->setText(str);

    //点击退出，显示的文本全部清空，按钮全部复位
    reset_radio();
    setButton_Enable_false();

    ui->textEdit_alone_text->clear();
    ui->label_3->clear();
    ui->label_4->clear();
    ui->label_5->clear();
    ui->label_6->clear();

    //清空题目容器
    lstExam.clear();

    emit alone_to_back();
}

void alonegame_interface::on_pushButton_begin_clicked()
{
    ui->pushButton_begin->setText("答题中...");
    ui->pushButton_begin->setEnabled(false);

    QString str = QString("第%1题，共10道题").arg(count);
    ui->label_3->setText(str);
    str = QString("%1分!").arg(score);
    ui->label_score->setText(str);

    ans_ch = show_test();

    setButton_Enable_true();

    emit alone_begin_game();
}

void alonegame_interface::get_list(QList<Exam_Info> lstExam)
{
    QList<Exam_Info>::iterator it;

    for(it = lstExam.begin(); it != lstExam.end(); it++)
    {
        this->lstExam.push_back(*it);
    }
}

void alonegame_interface::deal_time_out()
{
    if(second == 0)
    {
        answercheck(ans_ch);
        count++;
    }

    if(second == -1)
    {
        second = SECOND;
        ans_ch = show_test();
        reset_radio();
        ui->label_3->setText(QString("第%1题，共10道题").arg(count));
    }

    if(count == MAX_COUNT)
    {
        ui->pushButton->setText("提交");
    }

    if(count == MAX_COUNT + 1)
    {
        ui->pushButton->setEnabled(false);
        QMessageBox::information(this, "得分", QString("%1分!").arg(score));
        ui->pushButton_begin->setText("答题结束");

        emit count_over();
    }

    ui->label_score->setText(QString("%1分!").arg(score));
    ui->lcdNumber->display(second--);
}

void alonegame_interface::on_pushButton_clicked()
{
    answercheck(ans_ch);

    if(count != MAX_COUNT)
    {
        second = SECOND;       
        reset_radio();
        count++;

        if(count == MAX_COUNT)
        {
            ui->pushButton->setText("提交");
        }
    }
    else if(count == MAX_COUNT)
    {
        QString str = QString("%1分!").arg(score);             
        ui->pushButton_begin->setText("答题结束");
        ui->pushButton->setEnabled(false);

        emit count_over();

        QMessageBox::information(this, "得分", str);
    }

    QString str = QString("第%1题，共10道题").arg(count);
    ui->label_3->setText(str);

    ans_ch = show_test();
}

char alonegame_interface::show_test()
{
    QList<Exam_Info>::iterator it = lstExam.begin();
    int i = MAX_COUNT + 1 - count;
    while(i < MAX_COUNT)
    {
        it++;
        i++;
    }
    ui->textEdit_alone_text->setText(it->topic);
    ui->label_4->setText(it->optionA);
    ui->label_5->setText(it->optionB);
    ui->label_6->setText(it->optionC);

    QString str = QString("%1分!").arg(score);
    ui->label_score->setText(str);

    return (it->answer)[0];
}

void alonegame_interface::answercheck(char answer)
{
    switch (answer)
    {
        case 'A':
        {
            if(ui->radioButton_A->isChecked())
                score += 10;
            break;
        }
        case 'B':
        {
            if(ui->radioButton_B->isChecked())
                score += 10;
            break;
        }
        case 'C':
        {
            if(ui->radioButton_C->isChecked())
                score += 10;
            break;
        }
        default:
            break;
    }
}

void alonegame_interface::setButton_Enable_false()
{
    ui->pushButton->setEnabled(false);
    ui->radioButton_A->setCheckable(false);
    ui->radioButton_B->setCheckable(false);
    ui->radioButton_C->setCheckable(false);
}

void alonegame_interface::setButton_Enable_true()
{
    ui->pushButton->setEnabled(true);
    ui->radioButton_A->setCheckable(true);
    ui->radioButton_B->setCheckable(true);
    ui->radioButton_C->setCheckable(true);
}

void alonegame_interface::reset_radio()
{
    ui->radioButton_A->setAutoExclusive(false);
    ui->radioButton_B->setAutoExclusive(false);
    ui->radioButton_C->setAutoExclusive(false);
    ui->radioButton_A->setChecked(false);
    ui->radioButton_B->setChecked(false);
    ui->radioButton_C->setChecked(false);
    ui->radioButton_A->setAutoExclusive(true);
    ui->radioButton_B->setAutoExclusive(true);
    ui->radioButton_C->setAutoExclusive(true);
}
