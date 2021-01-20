#include "widget.h"
#include "myhead.h"

char user_Logined[15];  //保存已登录用户的用户名

/*------------------Init----------------*/
void Widget::deal_fromInit_toAlone()
{
    init_obj->hide();
    alg_obj->show();

    ExamLoad();     //加载题库
    Timer_init();   //初始化计时器
    alg_obj->get_list(lstExam);     //AloneGame界面获取题目

}

void Widget::deal_fromInit_toBattle()       //点击联网对战，进行网络连接
{
    init_obj->hide();
    log_obj->show();

    Tsocket->connectToHost(QHostAddress("127.0.0.1"), 8000);
    connect(Tsocket, &QTcpSocket::connected, log_obj, &login_interface::beConnection);
}

void Widget::deal_count_over()
{
    if(timer->isActive() == true)
    {
        timer->stop();
    }
}

void Widget::deal_fromInit_close()
{
    init_obj->close();
}

/*----------------AlongGame--------------*/
void Widget::deal_fromAlo_toBack()
{
    lstExam.clear();     //清空题目缓存
    if(timer->isActive() == true)
    {
        timer->stop();
    }

    alg_obj->hide();
    init_obj->show();
}

void Widget::deal_fromAlo_toGame()
{
    if(timer->isActive() == false)
        timer->start(1000);
}

/*------------------Login------------------*/
void Widget::deal_fromLogin_toBack()
{
    log_obj->hide();
    init_obj->show();

    Tsocket->disconnectFromHost();
}

void Widget::deal_fromLogin_tolog()
{
    if(log_obj->get_acct().length() != 0 && log_obj->get_pwd().length() != 0)
    {
        memset(&buffer, 0, sizeof(Cmd_Info));
        QByteArray tmp;
        strcpy(buffer.cmd, "login");

        tmp = log_obj->get_acct().toLocal8Bit();
        strcpy(buffer.user_cmd.username, tmp.data());
        tmp = log_obj->get_pwd().toLocal8Bit();
        strcpy(buffer.user_cmd.password, tmp.data());

//        strcpy(buffer.user_cmd.username, log_obj->get_acct().toUtf8().data());
//        strcpy(buffer.user_cmd.password, log_obj->get_pwd().toUtf8().data());

        log_obj->Edit_all_clear();

        Send_Data();
    }
}

void Widget::deal_fromLogin_toreg()
{
    log_obj->hide();
    reg_obj->show();
}

/*-----------------Reg------------------*/
void Widget::deal_fromReg_toBack()
{
    reg_obj->hide();
    log_obj->show();
}

void Widget::deal_fromReg_toReg()
{   
    if(reg_obj->get_acct().length() != 0 && reg_obj->get_pwd() != 0)
    {
        memset(&buffer, 0, sizeof(Cmd_Info));
        strcpy(buffer.cmd, "reg");

        strcpy(buffer.user_cmd.username, reg_obj->get_acct().toUtf8().data());
        strcpy(buffer.user_cmd.password, reg_obj->get_pwd().toUtf8().data());

        reg_obj->Edit_clear();

        Send_Data();
    }
}

/*------------------online-----------------*/
void Widget::deal_fromOnl_toBack()
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "exit_login");
    strcpy(buffer.user_cmd.username, user_Logined);

    Send_Data();

    onl_obj->hide();
    log_obj->show();

    memset(user_Logined, 0, sizeof(user_Logined));
}

void Widget::deal_fromOnl_toMatch()
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "match");
    strcpy(buffer.user_cmd.username, user_Logined);

    Send_Data();
}

void Widget::deal_fromOnl_toCancel()
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "cancel_match");
    strcpy(buffer.user_cmd.username, user_Logined);

    Send_Data();

    onl_obj->reset_pushButton();
}

/*-----------------BattleGame-------------*/
void Widget::deal_fromBtg_toGiveUp()
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "give_up");

    Send_Data();

    QMessageBox::information(btg_obj, "提示", "退出！");
    btg_obj->hide();
    onl_obj->show();
    onl_obj->reset_pushButton();
}

void Widget::deal_fromBtg_Over()
{
    QMessageBox::information(btg_obj, "提示", "退出！");
    btg_obj->hide();
    onl_obj->show();
    onl_obj->reset_pushButton();
}

void Widget::deal_fromBtg_getExam()
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "get_exam");

    Send_Data();
}

void Widget::deal_fromBtg_Score()  //------------------
{
    memset(&buffer, 0, sizeof(Cmd_Info));
    strcpy(buffer.cmd, "send_score");
    buffer.user_cmd.score = btg_obj->ret_socre();

//    qDebug() << "score:" << buffer.user_cmd.score;

    Send_Data();
}

/*-----------------TcpSocket---------------*/
void Widget::Read_Data()        //接收服务器数据
{
    Result_msg ret_msg;
    Tsocket->read((char *)&ret_msg, sizeof(Result_msg));

    Deal_Result(ret_msg);       //处理结果
}

void Widget::Send_Data()        //向服务器发送数据
{
//    qDebug() << buffer.cmd;
//    qDebug() << buffer.user_cmd.username;
//    qDebug() << buffer.user_cmd.password;
    Tsocket->write((char *)&buffer, sizeof(Cmd_Info));
}

void Widget::Deal_Result(Result_msg tmp)
{
    Result_msg ret_msg = tmp;

    if(strcmp(ret_msg.result, "user_exist") == 0)
    {
        QMessageBox::information(reg_obj, "提示", "该用户名已被注册！");
    }
    else if(strcmp(ret_msg.result, "reg_success") == 0)
    {
        QMessageBox::information(reg_obj, "提示", "注册成功！");
        reg_obj->hide();
        log_obj->show();
    }
    else if(strcmp(ret_msg.result, "password_error") == 0)
    {
        log_obj->Edit_pwd_clear();
        QMessageBox::information(log_obj, "提示", "密码错误");
    }
    else if(strcmp(ret_msg.result, "login_userLogined") == 0)
    {
        QMessageBox::information(log_obj, "提示", "不可重复登录");
    }
    else if(strcmp(ret_msg.result, "login_success") == 0)
    {
        QMessageBox::information(log_obj, "提示", "登录成功");
        strcpy(user_Logined, buffer.user_cmd.username);
        log_obj->hide();
        onl_obj->show();
        onl_obj->get_online_user(user_Logined);
    }
    else if(strcmp(ret_msg.result, "user_unexist") == 0)
    {
        QMessageBox::information(log_obj, "提示", "该用户尚未注册");
    }
    else if(strcmp(ret_msg.result, "match_success") == 0)
    {
        //QMessageBox::information(onl_obj, "提示", "匹配成功，点击进入对战界面");
        onl_obj->hide();
        btg_obj->show();
        btg_obj->get_against_name(ret_msg.username);
        btg_obj->start_timer();
    }
    else if(strcmp(ret_msg.result, "send_exam") == 0)
    {
        Exam_Info tmp;
        tmp.id = ret_msg.exam_info.id;
        strcpy(tmp.topic, ret_msg.exam_info.topic);
        strcpy(tmp.optionA, ret_msg.exam_info.optionA);
        strcpy(tmp.optionB, ret_msg.exam_info.optionB);
        strcpy(tmp.optionC, ret_msg.exam_info.optionC);
        strcpy(tmp.answer, ret_msg.exam_info.answer);

        btg_obj->get_exam(tmp);
    }
    else if(strcmp(ret_msg.result, "recv_score") == 0)
    {
        btg_obj->get_against_socre(ret_msg.score);
//        qDebug() << ret_msg.score;
    }
    else if(strcmp(ret_msg.result, "a_give_up") == 0)
    {
        btg_obj->reset();
        QMessageBox::information(btg_obj, "提示", "对手退出");
        btg_obj->hide();
        onl_obj->reset_pushButton();
        onl_obj->show();
    }

}

