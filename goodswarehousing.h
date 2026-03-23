#ifndef GOODSWAREHOUSING_H
#define GOODSWAREHOUSING_H

#include <QWidget>
#pragma execution_character_set("utf-8");

//数据库相关头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QMessageBox>
#include <QDateTime>

namespace Ui {
class goodswarehousing;
}

class goodswarehousing : public QWidget
{
    Q_OBJECT

public:
    explicit goodswarehousing(QWidget *parent = nullptr);
    ~goodswarehousing();

private:
    void on_pushButton_InputGoods_clicked();
    void on_pushButton_Exit_clicked();

private:
    Ui::goodswarehousing *ui;

public:
    void InitComboBoxFunc();
};

#endif // GOODSWAREHOUSING_H
