#ifndef ADDCOMMODITY_H
#define ADDCOMMODITY_H

#include <QDialog>


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include <QDateTime>
#include <QMessageBox>
namespace Ui {
class AddCommodity;
}

class AddCommodity : public QDialog
{
    Q_OBJECT

public:
    explicit AddCommodity(QWidget *parent = nullptr);
    ~AddCommodity();

private slots:
    void on_pushButton_Exit_clicked();

    void on_pushButton_Add_clicked();
signals:
    void dataChanged();// 数据变化信号

private:
    Ui::AddCommodity *ui;
};

#endif // ADDCOMMODITY_H
