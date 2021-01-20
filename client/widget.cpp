#include "slots_function.cpp"
//#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    memset(&buffer, 0, sizeof(Cmd_Info));

    Mysql_init();   //初始化数据库  
    Net_init();

    init_obj = new init_interface;
    alg_obj = new alonegame_interface;
    log_obj = new login_interface;
    reg_obj = new reg_interface;
    onl_obj = new online_interface;
    btg_obj = new battlegame_interface;

    init_obj->show();
    connect(init_obj,   &init_interface::init_to_alone,         this,   &Widget::deal_fromInit_toAlone  );
    connect(init_obj,   &init_interface::init_to_battle,        this,   &Widget::deal_fromInit_toBattle );
    connect(init_obj,   &init_interface::init_close,            this,   &Widget::deal_fromInit_close    );

    connect(alg_obj,    &alonegame_interface::alone_to_back,    this,   &Widget::deal_fromAlo_toBack    );
    connect(alg_obj,    &alonegame_interface::alone_begin_game, this,   &Widget::deal_fromAlo_toGame    );
    connect(alg_obj,    &alonegame_interface::count_over,       this,   &Widget::deal_count_over);

    connect(log_obj,    &login_interface::login_to_back,        this,   &Widget::deal_fromLogin_toBack  );
    connect(log_obj,    &login_interface::login_to_log,         this,   &Widget::deal_fromLogin_tolog   );
    connect(log_obj,    &login_interface::login_to_reg,         this,   &Widget::deal_fromLogin_toreg   );

    connect(reg_obj,    &reg_interface::reg_to_reg,             this,   &Widget::deal_fromReg_toReg     );
    connect(reg_obj,    &reg_interface::reg_to_back,            this,   &Widget::deal_fromReg_toBack    );

    connect(onl_obj,    &online_interface::online_to_back,      this,   &Widget::deal_fromOnl_toBack    );
    connect(onl_obj,    &online_interface::online_to_match,     this,   &Widget::deal_fromOnl_toMatch   );
    connect(onl_obj,    &online_interface::online_to_cancel,    this,   &Widget::deal_fromOnl_toCancel  );

    connect(btg_obj,    &battlegame_interface::btg_to_giveup,   this,   &Widget::deal_fromBtg_toGiveUp  );
    connect(btg_obj,    &battlegame_interface::btg_to_Over,     this,   &Widget::deal_count_over        );
    connect(btg_obj,    &battlegame_interface::btg_get_exam,    this,   &Widget::deal_fromBtg_getExam   );
    connect(btg_obj,    &battlegame_interface::btg_send_score,  this,   &Widget::deal_fromBtg_Score     );  //-------------------------
}

void Widget::Mysql_init()
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");      //设置字符编码，使QT能够正常从MySQL中读写汉字
    QTextCodec::setCodecForLocale(codec);
    db.exec("set NAMES UFT8");

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());      //解决 Qstring 中含有中文时转为 char * 时出现乱码的问题

    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("123456");
    db.setDatabaseName("project");
}

void Widget::Net_init()
{
    Tsocket = new QTcpSocket(this);     //通信套接字
    Tsocket->abort();       //取消原有连接

    connect(Tsocket,    &QTcpSocket::readyRead,     this,   &Widget::Read_Data);
}

void Widget::Timer_init()
{
    timer = new QTimer(this);
    connect(timer,   &QTimer::timeout,  alg_obj,    &alonegame_interface::deal_time_out);
}

void Widget::ExamLoad()
{
    QByteArray ba;      //QString 转 Char * 时的中间变量
    QSqlQuery query;
    QString sql = ("SELECT * FROM examlist ORDER BY RAND() LIMIT 10");

    if(db.open() == true) {/*qDebug() << "DataBase Open Success";*/}    //打开数据库并执行sql语句
    query.exec(sql);

    while(query.next())
    {
        exam_content = new Exam_Info;
        memset(exam_content, 0, sizeof(Exam_Info));

        exam_content->id = query.value(0).toInt();          //记录题号id

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
}

Widget::~Widget()
{
    delete init_obj;
    delete alg_obj;
    delete log_obj;
    delete reg_obj;
    delete onl_obj;
    delete btg_obj;

    delete ui;
}
