#include "mythread.h"

mythread::mythread(QTcpSocket *client_socket, QThread *parent) : QThread(parent)
{
    this->client_socket = client_socket;

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());      //解决 Qstring 中含有中文时转为 char * 时出现乱码的问题
}

void mythread::run()
{
    connect(client_socket, SIGNAL(readyRead()), this, SLOT(deal_readData()));
    connect(client_socket, SIGNAL(disconnected()), this, SLOT(deal_client_disConnect()));

    //this->exec();
}

void mythread::MySql_Init_Open()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))         //设置数据库连接名
      db = QSqlDatabase::database("qt_sql_default_connection");
    else
      db = QSqlDatabase::addDatabase("QMYSQL");

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");      //设置字符编码，使QT能够正常从MySQL中读写汉字
    QTextCodec::setCodecForLocale(codec);
    db.exec("set NAMES UFT8");

    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("project");
    /*bool open_ok = */db.open();
//    if(open_ok)
//        qDebug() <<"open database success";
//    else
//        qDebug() <<"open database fail";
}

void mythread::deal_readData()
{
    Cmd_Info tmp_cmd;           //命令信息
    memset(&tmp_cmd, 0, sizeof(Cmd_Info));
    client_socket->read((char *)&tmp_cmd, sizeof(Cmd_Info));

    //qDebug() << tmp_cmd.cmd;

    if(strcmp(tmp_cmd.cmd, "reg") == 0)                 //注册
        deal_reg(tmp_cmd.user_cmd.username, tmp_cmd.user_cmd.password);

    else if(strcmp(tmp_cmd.cmd, "login") == 0)          //登录
        deal_log(tmp_cmd.user_cmd.username, tmp_cmd.user_cmd.password);

    else if(strcmp(tmp_cmd.cmd, "exit_login") == 0)     //退出登录
        deal_exit(tmp_cmd.user_cmd.username);

    else if(strcmp(tmp_cmd.cmd, "match") == 0)          //匹配
        deal_match(tmp_cmd.user_cmd.username);

    else if(strcmp(tmp_cmd.cmd, "cancel_match") == 0)   //  取消匹配
        deal_cancel_match(tmp_cmd.user_cmd.username);

    else if(strcmp(tmp_cmd.cmd, "get_exam") == 0)
        send_exam();

    else if(strcmp(tmp_cmd.cmd, "send_score") == 0)     //转发分数
        send_score_fun(tmp_cmd.user_cmd.score);

    else if(strcmp(tmp_cmd.cmd, "give_up") == 0)
        send_giveUp_fun();
}

void mythread::deal_client_disConnect()
{
    emit send_disConnect_socket(client_socket);
}

void mythread::deal_reg(char *username, char *password)
{
    MySql_Init_Open();
    QSqlQuery query;
    QString sql;
    Result_msg ret_msg;         //处理结果

    memset(ret_msg.result, 0 , sizeof(Result_msg));

    sql = QString("SELECT name FROM userinfo where name = \"%1\"").arg(username);
    query.exec(sql);
    if(query.size() > 0)
    {
        strcpy(ret_msg.result, "user_exist");
    }
    else
    {
        strcpy(ret_msg.result, "reg_success");
        sql = QString("INSERT INTO userinfo values(\"%1\", \"%2\", 0, 0)").arg(username).arg(password);
        query.exec(sql);
    }

    emit send_user_result(username, ret_msg.result);
    client_socket->write((char *)&ret_msg, sizeof(Result_msg));

    db.close();
}

void mythread::deal_log(char *username, char *password)
{
    MySql_Init_Open();
    QSqlQuery query;
    QString sql;
    Result_msg ret_msg;

    memset(ret_msg.result, 0, sizeof(Result_msg));

//    qDebug() << username;
//    qDebug() << password;

    sql = QString("select * from userinfo where name = \"%1\";").arg(username);
    query.exec(sql);

    if(query.next())
    {
        QByteArray tmp = query.value(1).toString().toLocal8Bit();
        if(strcmp(tmp.data(), password) == 0)
        {
            if(query.value(2).toInt() == 1)
            {
                strcpy(ret_msg.result, "login_userLogined");
            }
            else
            {
                strcpy(ret_msg.result, "login_success");
                sql = QString("update userinfo set isLogin = 1 where name = \"%1\";").arg(username);
                query.exec(sql);

                emit send_user_login(client_socket, username);
            }
        }
        else
        {
            strcpy(ret_msg.result, "password_error");
        }
    }
    else
    {
        strcpy(ret_msg.result, "user_unexist");
    }

    emit send_user_result(username, ret_msg.result);
    client_socket->write((char *)&ret_msg, sizeof(Result_msg));
//    qDebug() << ret_msg.result;

    db.close();
}

void mythread::deal_exit(char *username)
{
    MySql_Init_Open();
    QSqlQuery query;
    QString sql;
    Result_msg ret_msg;

    sql = QString("update userinfo set isLogin = 0 where name = \"%1\";").arg(username);
    query.exec(sql);

    strcpy(ret_msg.result, "exit_login");
    emit send_user_result(username, ret_msg.result);
    emit send_user_exitLogin(client_socket, username);     //退出登录
    emit send_user_cancel(client_socket, username);     //退出登录的同时取消匹配

    db.close();
}

void mythread::deal_match(char *username)
{
    Result_msg ret_msg;
    strcpy(ret_msg.result, "match");

    emit send_user_result(username, ret_msg.result);
    emit send_user_match(client_socket, username);
}

void mythread::deal_cancel_match(char *username)
{
    Result_msg ret_msg;
    strcpy(ret_msg.result, "cancel_match");

    emit send_user_result(username, ret_msg.result);
    emit send_user_cancel(client_socket, username);
}

void mythread::get_matchUser_info(QString username)
{
    against_username = username;

    QByteArray tmp;
    Result_msg ret_msg;
    tmp = username.toLocal8Bit();

    strcpy(ret_msg.result, "match_success");
    strcpy(ret_msg.username, tmp);

    client_socket->write((char *)&ret_msg, sizeof(Result_msg));
}

void mythread::get_lstExam(QList<Exam_Info> tmp_lst)
{
    lstExam = tmp_lst;

//    qDebug() << "thread lstsize:" << lstExam.size();
}

void mythread::send_exam()
{
    Result_msg ret_msg;
    QList<Exam_Info>::iterator it = lstExam.begin();

    strcpy(ret_msg.result, "send_exam");
    ret_msg.exam_info.id = it->id;
    strcpy(ret_msg.exam_info.topic, it->topic);
    strcpy(ret_msg.exam_info.optionA, it->optionA);
    strcpy(ret_msg.exam_info.optionB, it->optionB);
    strcpy(ret_msg.exam_info.optionC, it->optionC);
    strcpy(ret_msg.exam_info.answer, it->answer);

    client_socket->write((char *)&ret_msg, sizeof(Result_msg));

    lstExam.erase(it);
//    qDebug() << "size :" << lstExam.size();
}

void mythread::send_score_fun(int score)
{
//    qDebug() << client_socket << score << against_username;
    emit send_score(score, against_username);
}

void mythread::get_matchUser_score(int score)
{
    Result_msg ret_msg;
    strcpy(ret_msg.result, "recv_score");
    ret_msg.score = score;

//    qDebug() << client_socket << score;

    client_socket->write((char *)&ret_msg, sizeof(Result_msg));
}

void mythread::send_giveUp_fun()
{
    emit send_give_up(against_username);
}

void mythread::send_giveUp_toClient()
{
    Result_msg ret_msg;
    strcpy(ret_msg.result, "a_give_up");

    client_socket->write((char *)&ret_msg, sizeof(Result_msg));
}
