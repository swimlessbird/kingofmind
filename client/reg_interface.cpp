#include "reg_interface.h"
#include "ui_reg_interface.h"
#include <QMessageBox>

reg_interface::reg_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reg_interface)
{
    ui->setupUi(this);
    setWindowTitle("注册");

    QRegExp acctExp("[A-Za-z0-9_]{1,15}");               //设置可输入字符以及可输入最大长度
    ui->lineEdit_reg_acct->setValidator(new QRegExpValidator(acctExp, this));
    QRegExp psdExp("[A-Za-z0-9_]{1,12}");
    ui->lineEdit_reg_pwd->setValidator(new QRegExpValidator(psdExp, this));
    ui->lineEdit_reg_repwd->setValidator(new QRegExpValidator(psdExp, this));

    ui->lineEdit_reg_acct->setPlaceholderText("请输入用户名");
    ui->lineEdit_reg_pwd->setPlaceholderText("请输入密码");
    ui->lineEdit_reg_repwd->setPlaceholderText("请确认密码");
    ui->lineEdit_reg_pwd->setEchoMode(QLineEdit::Password);
    ui->lineEdit_reg_repwd->setEchoMode(QLineEdit::Password);
}

reg_interface::~reg_interface()
{
    delete ui;
}

void reg_interface::on_pushButton_reg_back_clicked()
{
    emit reg_to_back();
}

void reg_interface::on_pushButton_reg_reg_clicked()
{
    if( !(ui->lineEdit_reg_acct->text().length() && ui->lineEdit_reg_pwd->text().length() && ui->lineEdit_reg_repwd->text().length()) )
    {
        ui->lineEdit_reg_pwd->clear();
        ui->lineEdit_reg_repwd->clear();
        QMessageBox::information(this, "提示：", "请输入完整信息！");
    }
    else if(ui->lineEdit_reg_pwd->text() != ui->lineEdit_reg_repwd->text())
    {
        ui->lineEdit_reg_pwd->clear();
        ui->lineEdit_reg_repwd->clear();
        QMessageBox::information(this, "提示：", "两次密码不匹配！");
    }
    else
    {
        emit reg_to_reg();
    }
}

QString reg_interface::get_acct()
{
    QString tmp = ui->lineEdit_reg_acct->text();

    return tmp;
}

QString reg_interface::get_pwd()
{
    QString tmp = ui->lineEdit_reg_pwd->text();

    return tmp;
}

void reg_interface::Edit_clear()
{
    ui->lineEdit_reg_acct->clear();
    ui->lineEdit_reg_pwd->clear();
    ui->lineEdit_reg_repwd->clear();
}
