#include "widget.h"
#include "ui_widget.h"
#include "mythread.h"
#include <QHostAddress>
#include <QDateTime>

#define MAX_COUNT 10

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("服务器Port:8000");

    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 8000);

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptClient()));
}

void Widget::acceptClient()
{
    while(tcpServer->hasPendingConnections())
    {
        QTcpSocket *tcpSocket = new QTcpSocket(this);
        tcpSocket = tcpServer->nextPendingConnection();
        mythread *c_thread = new mythread(tcpSocket);
        lstThread.insert(tcpSocket, c_thread);

//        qDebug() << "thread size:" << lstThread.size();

        connect(c_thread,   SIGNAL(send_user_result(char*,char*)),          this,   SLOT(deal_thread_result(char*,char*))           );
        connect(c_thread,   SIGNAL(send_user_match(QTcpSocket*,char*)),     this,   SLOT(deal_thread_match(QTcpSocket*,char*))      );
        connect(c_thread,   SIGNAL(send_user_login(QTcpSocket*,char*)),     this,   SLOT(deal_user_login(QTcpSocket*,char*))        );
        connect(c_thread,   SIGNAL(send_user_cancel(QTcpSocket*,char*)),    this,   SLOT(deal_thread_cancel(QTcpSocket*,char*))     );
        connect(c_thread,   SIGNAL(send_disConnect_socket(QTcpSocket*)),    this,   SLOT(deal_thread_disConnect(QTcpSocket*))       );
        connect(c_thread,   SIGNAL(send_user_exitLogin(QTcpSocket*,char*)), this,   SLOT(deal_thread_exitLogin(QTcpSocket*,char*))  );
        connect(c_thread,   SIGNAL(send_score(int,QString)),                this,   SLOT(tran_score(int,QString))                   );
        connect(c_thread,   SIGNAL(send_give_up(QString)),                  this,   SLOT(tran_giveup(QString))                      );

        c_thread->start();
    }
}

void Widget::deal_thread_result(char *username, char *result)
{
    ui->textEdit_Log->append(QDateTime::currentDateTime().toString());
    ui->textEdit_Log->append(tr("用户：%1   操作结果：%2").arg(username).arg(result));
    ui->textEdit_Log->append(" ");
}

void Widget::deal_thread_match(QTcpSocket *client_socket, char *username)
{
    QString str = QString(username);
    mutex.lock();
    lstMatch.insert(client_socket, str);
    check_lstMatch();
    mutex.unlock();
}

void Widget::deal_user_login(QTcpSocket *client_socket, char *username)
{
    QString str = QString(username);
    lstOnline.insert(client_socket, str);

//    qDebug() << "online size:" << lstOnline.size();
}

void Widget::check_lstMatch()
{
    if(lstMatch.size() > 1)
    {
        ui->textEdit_Log->append(tr("匹配成功："));

        QMap<QTcpSocket *, QString>::iterator it = lstMatch.begin();
        while(it != lstMatch.end())
        {
            ui->textEdit_Log->append(it.value());
            it++;
        }

        send_matchUser_toThread();

        lstMatch.clear();

        ui->textEdit_Log->append(" ");
    }
}

void Widget::send_matchUser_toThread()
{
    QMap<QTcpSocket *, QString>::iterator it_match = lstMatch.begin();
//    QMap<QTcpSocket *, mythread *>::iterator it_thread = lstThread.begin();

    QTcpSocket *socket_one = it_match.key();
    QString username_one = it_match.value();
    it_match++;
    QTcpSocket *socket_two = it_match.key();
    QString username_two = it_match.value();

    get_Topic();
//    qDebug() << "size :" << lstExam.size();
    lstThread.find(socket_one).value()->get_matchUser_info(username_two);
    lstThread.find(socket_one).value()->get_lstExam(lstExam);
    lstThread.find(socket_two).value()->get_matchUser_info(username_one);
    lstThread.find(socket_two).value()->get_lstExam(lstExam);

    lstExam.clear();
}

void Widget::deal_thread_cancel(QTcpSocket *client_socket, char *username)
{
    QString s_name = QString(username);
    QMap<QTcpSocket *, QString>::iterator it = lstMatch.find(client_socket);;

   if(it != lstMatch.end() && it.value() == s_name)
   {
       mutex.lock();
       lstMatch.erase(it);
       mutex.unlock();
   }
}

void Widget::deal_thread_disConnect(QTcpSocket *client_socket)
{
    QMap<QTcpSocket *, mythread *>::iterator it = lstThread.find(client_socket);

    if(it != lstThread.end())
    {
        it.value()->quit();
        mutex.lock();
        lstThread.erase(it);
        mutex.unlock();
    }

//    qDebug() << "thread size:" << lstThread.size();
}

void Widget::deal_thread_exitLogin(QTcpSocket *client_socket, char *username)
{
    QString s_name = QString(username);
    QMap<QTcpSocket *, QString>::iterator it = lstOnline.find(client_socket);

    if(it != lstOnline.end() && it.value() == s_name)
    {
        mutex.lock();
        lstOnline.erase(it);
        mutex.unlock();
    }

//    qDebug() << "online size:" << lstOnline.size();
}

void Widget::Mysql_Init()
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
}

void Widget::get_Topic()
{
    Mysql_Init();

    QByteArray ba;      //QString 转 Char * 时的中间变量
    QSqlQuery query;
    QString sql = QString("SELECT * FROM examlist ORDER BY RAND() LIMIT 10;");

    if(db.open() == true) {/*qDebug() << "DataBase Open Success";*/}    //打开数据库并执行sql语句
        query.exec(sql);

    while(query.next())
    {
        Exam_Info *exam_content = new Exam_Info;
        memset(exam_content, 0, sizeof(Exam_Info));

        exam_content->id = query.value(0).toInt();          //记录题号id
//        qDebug() << exam_content->id;

        ba = query.value(1).toString().toLocal8Bit();       //记录题目topic
        strcpy(exam_content->topic, ba.data());

        ba = query.value(2).toString().toLocal8Bit();       //记录选项A optionA
        strcpy(exam_content->optionA, ba.data());

        ba = query.value(3).toString().toLocal8Bit();       //记录选项B optionA
        strcpy(exam_content->optionB, ba.data());

        ba = query.value(4).toString().toLocal8Bit();       //记录选项B optionA
        strcpy(exam_content->optionC, ba.data());

        ba = query.value(5).toString().toLocal8Bit();       //记录答案 answer
        strcpy(exam_content->answer, ba.data());

        lstExam.push_back(*exam_content);       //插入list表中

        delete exam_content;
    }

    db.close();

    qDebug() << "size:" << lstExam.size();
}

void Widget::tran_score(int score, QString username)
{
    search_thread(username)->get_matchUser_score(score);
}

void Widget::tran_giveup(QString username)
{
    search_thread(username)->send_giveUp_toClient();
}

mythread * Widget::search_thread(QString username)
{
    QMap<QTcpSocket *, QString>::iterator it_online = lstOnline.begin();
    QMap<QTcpSocket *, mythread *>::iterator it_thread = lstThread.begin();
    QTcpSocket *tmp_socket;

    while(it_online != lstOnline.end())
    {
        if(it_online.value() == username)
        {
            tmp_socket = it_online.key();
            break;
        }
        it_online++;
    }

    while(it_thread != lstThread.end())
    {
        if(tmp_socket == it_thread.key())
        {
            return it_thread.value();
            break;
        }
        it_thread++;
    }
}

void Widget::on_pushButton_clicked()
{
    ui->textEdit_Log->clear();
}
