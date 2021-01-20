#include "online_interface.h"
#include "ui_online_interface.h"

online_interface::online_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::online_interface)
{
    ui->setupUi(this);
    setWindowTitle("对战大厅");

    ui->pushButton_cancel->setVisible(false);
    ui->pushButton_cancel->setEnabled(false);
}

online_interface::~online_interface()
{
    delete ui;
}

void online_interface::on_pushButton_exit_clicked()
{
    emit online_to_back();
}

void online_interface::on_pushButton_match_clicked()
{
    ui->pushButton_match->setText("匹配中...");
    ui->pushButton_match->setEnabled(false);
    ui->pushButton_cancel->setEnabled(true);
    ui->pushButton_cancel->setVisible(true);
    emit online_to_match();
}

void online_interface::get_online_user(char *username)
{
    QString str = QString("当前登录用户：%1").arg(username);
    ui->label->setText(str);
}

void online_interface::reset_pushButton()
{
    ui->pushButton_match->setText("开始匹配");
    ui->pushButton_match->setEnabled(true);
    ui->pushButton_cancel->setEnabled(false);
    ui->pushButton_cancel->setVisible(false);
}

void online_interface::on_pushButton_cancel_clicked()
{
    emit online_to_cancel();
}
