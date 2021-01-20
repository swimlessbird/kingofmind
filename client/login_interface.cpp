#include "login_interface.h"
#include "ui_login_interface.h"
#include <QMessageBox>
#include <QDebug>

login_interface::login_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login_interface)
{
    ui->setupUi(this);
    setWindowTitle("登录");

    QRegExp acctExp("[A-Za-z0-9_]{1,15}");               //设置可输入字符以及可输入最大长度
    ui->lineEdit_log_acct->setValidator(new QRegExpValidator(acctExp, this));
    QRegExp psdExp("[A-Za-z0-9_]{1,12}");
    ui->lineEdit_log_pwd->setValidator(new QRegExpValidator(psdExp, this));

    ui->lineEdit_log_acct->setPlaceholderText("请输入用户名");
    ui->lineEdit_log_pwd->setPlaceholderText("请输入密码");
    ui->lineEdit_log_pwd->setEchoMode(QLineEdit::Password);

    ui->pushButton_log_log->setEnabled(false);
    ui->pushButton_log_reg->setEnabled(false);
}

login_interface::~login_interface()
{
    delete ui;
}

void login_interface::on_pushButton_log_back_clicked()
{
    emit login_to_back();
}

void login_interface::on_pushButton_log_log_clicked()
{
    if( !(ui->lineEdit_log_acct->text().length() && ui->lineEdit_log_pwd->text().length()) )
    {
        QMessageBox::information(this, "提示", "请输入完整信息");
    }
    else
    {
        emit login_to_log();
    }
}

void login_interface::on_pushButton_log_reg_clicked()
{
    emit login_to_reg();
}

QString login_interface::get_acct()
{
    QString acct = ui->lineEdit_log_acct->text();

    return acct;
}

QString login_interface::get_pwd()
{
    QString pwd = ui->lineEdit_log_pwd->text();    

    return pwd;
}

void login_interface::beConnection()
{
    ui->pushButton_log_log->setEnabled(true);
    ui->pushButton_log_reg->setEnabled(true);
    ui->label_4->setText("已成功连接服务器");
}

void login_interface::Edit_pwd_clear()
{
    ui->lineEdit_log_pwd->clear();
}

void login_interface::Edit_all_clear()
{
    ui->lineEdit_log_acct->clear();
    ui->lineEdit_log_pwd->clear();
}
