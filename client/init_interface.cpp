#include "init_interface.h"
#include "ui_init_interface.h"

init_interface::init_interface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::init_interface)
{
    ui->setupUi(this);
    setWindowTitle("选择模式");
}

init_interface::~init_interface()
{
    delete ui;
}

void init_interface::on_pushButton_alone_clicked()  //发送“单人模式”信号
{
    emit init_to_alone();
}

void init_interface::on_pushButton_battle_clicked() //发送“联网对战”信号
{
    emit init_to_battle();
}

void init_interface::on_pushButton_close_clicked()  //发送“退出”信号
{
    emit init_close();
}
