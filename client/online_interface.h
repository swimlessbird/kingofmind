#ifndef ONLINE_INTERFACE_H
#define ONLINE_INTERFACE_H

#include <QWidget>

namespace Ui {
class online_interface;
}

class online_interface : public QWidget
{
    Q_OBJECT

public:
    explicit online_interface(QWidget *parent = 0);
    ~online_interface();

    void get_online_user(char *username);   //获取当前登录的用户名

    void reset_pushButton();

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_match_clicked();

    void on_pushButton_cancel_clicked();

signals:
    void online_to_back();
    void online_to_match();
    void online_to_cancel();

private:
    Ui::online_interface *ui;
};

#endif // ONLINE_INTERFACE_H
