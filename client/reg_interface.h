#ifndef REG_INTERFACE_H
#define REG_INTERFACE_H

#include <QWidget>

namespace Ui {
class reg_interface;
}

class reg_interface : public QWidget
{
    Q_OBJECT

public:
    explicit reg_interface(QWidget *parent = 0);
    ~reg_interface();

    QString get_acct();
    QString get_pwd();
    void Edit_clear();

private slots:
    void on_pushButton_reg_back_clicked();

    void on_pushButton_reg_reg_clicked();

signals:
    void reg_to_back();
    void reg_to_reg();

private:
    Ui::reg_interface *ui;
};

#endif // REG_INTERFACE_H
