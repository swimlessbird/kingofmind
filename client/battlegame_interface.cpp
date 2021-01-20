#include "battlegame_interface.h"
#include "ui_battlegame_interface.h"

#define SECOND 10
#define Pre_SECOND 3

int battlegame_interface::second = Pre_SECOND - 1;
int battlegame_interface::m_score = 0;
int battlegame_interface::count = 1;

battlegame_interface::battlegame_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::battlegame_interface)
{
    ui->setupUi(this);
    setWindowTitle("联网对战");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &battlegame_interface::deal_time_out);

    reset();
}

battlegame_interface::~battlegame_interface()
{
    delete ui;
}

void battlegame_interface::on_pushButton_surrender_clicked()
{   
//    if(timer->isActive() == true)
//    {
//        timer->stop();
//        emit btg_to_giveup();
//    }

//    if(timer->isActive() == false)
//    {
//        emit btg_to_Over();
//    }

    reset();
    emit btg_to_giveup();
}

void battlegame_interface::get_against_name(char *username)
{
    QString str = QString("对手：%1").arg(QString(username));
    ui->label_against_username->setText(str);
}

void battlegame_interface::get_against_socre(int score)
{
    ui->label_Against_score->setText(QString("%1分").arg(score));
}

void battlegame_interface::start_timer()
{
    timer->start(1000);
}

void battlegame_interface::deal_time_out()
{
    if(second == 0 && count <= 10)
    {
        emit btg_get_exam();

        check_answer(answer);       //假设count = 10， 检查count = 9时的题目答案
    }
    else if(second == -1 && count <= 10)
    {
        ui->label_num->setText(QString("第 %1 题，共 10 道题").arg(count));
        ui->label_Your_score->setText(QString("%1分").arg(m_score));
        answer = tmp_exam.answer[0];
        qDebug() << "answer: " << answer;

        reset_radio();
        show_exam();

        second = SECOND;
        count++;
    }
    else if(second == 0 && count == 11)
    {        
        check_answer(answer);
        ui->label_Your_score->setText(QString("%1分").arg(m_score));
        set_radioEnable_false();
    }

    if(second == SECOND)
    {
        emit btg_send_score();
    }

    if(count <= 11)
    {
        ui->lcdNumber->display(second--);
        if(count == 11 && second == -1)
        {
            count++;
        }
    }
    else if(count == 12)
    {
        emit btg_send_score();
        timer->stop();
    }
}


void battlegame_interface::check_answer(char answer)
{
    switch (answer)
    {
        case 'A':
        {
            if(ui->radioButton_A->isChecked())
                m_score += 10;
            break;
        }
        case 'B':
        {
            if(ui->radioButton_B->isChecked())
                m_score += 10;
            break;
        }
        case 'C':
        {
            if(ui->radioButton_C->isChecked())
                m_score += 10;
            break;
        }
        default:
            break;
    }
}

void battlegame_interface::get_exam(Exam_Info tmp)
{
    tmp_exam = tmp;
}

int battlegame_interface::ret_socre()
{
    return m_score;
}

void battlegame_interface::show_exam()
{
    ui->textEdit->setText(tmp_exam.topic);
    ui->label_Option_A->setText(tmp_exam.optionA);
    ui->label_Option_B->setText(tmp_exam.optionB);
    ui->label_Option_C->setText(tmp_exam.optionC);
}

void battlegame_interface::reset()
{
    if(timer->isActive())
        timer->stop();

    ui->label_Option_A->clear();
    ui->label_Option_B->clear();
    ui->label_Option_C->clear();
    ui->textEdit->clear();

    reset_radio();
    set_radioEnable_true();
    ui->label_num->clear();
    ui->label_Your_score->clear();
    ui->label_against_username->clear();
    ui->label_Against_score->clear();

    ui->lcdNumber->display(Pre_SECOND);
    second = Pre_SECOND - 1;
    count = 1;
    m_score = 0;
}

void battlegame_interface::reset_radio()
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

void battlegame_interface::set_radioEnable_true()
{
    ui->radioButton_A->setEnabled(true);
    ui->radioButton_B->setEnabled(true);
    ui->radioButton_C->setEnabled(true);
}

void battlegame_interface::set_radioEnable_false()
{
    ui->radioButton_A->setEnabled(false);
    ui->radioButton_B->setEnabled(false);
    ui->radioButton_C->setEnabled(false);
}
