#ifndef LOGIN_INTERFACE_H
#define LOGIN_INTERFACE_H

#include <QWidget>

namespace Ui {
class login_interface;
}

class login_interface : public QWidget
{
    Q_OBJECT

public:
    explicit login_interface(QWidget *parent = 0);
    ~login_interface();

    QString get_acct();
    QString get_pwd();

    void Edit_pwd_clear();
    void Edit_all_clear();

private slots:
    void on_pushButton_log_back_clicked();

    void on_pushButton_log_log_clicked();

    void on_pushButton_log_reg_clicked();

public slots:
    void beConnection();

signals:
    void login_to_back();
    void login_to_reg();
    void login_to_log();

private:
    Ui::login_interface *ui;   
};

#endif // LOGIN_INTERFACE_H
