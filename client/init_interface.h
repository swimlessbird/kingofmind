#ifndef INIT_INTERFACE_H
#define INIT_INTERFACE_H

#include <QWidget>

namespace Ui {
class init_interface;
}

class init_interface : public QWidget
{
    Q_OBJECT

public:
    explicit init_interface(QWidget *parent = 0);
    ~init_interface();

private:
    Ui::init_interface *ui;

signals:
    void init_to_alone();
    void init_to_battle();
    void init_close();

private slots:
    void on_pushButton_alone_clicked();
    void on_pushButton_battle_clicked();
    void on_pushButton_close_clicked();
};

#endif // INIT_INTERFACE_H
