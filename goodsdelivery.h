#ifndef GOODSDELIVERY_H
#define GOODSDELIVERY_H

#include <QWidget>
// 数据库相关头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QtDebug>
#include <QMessageBox>
#include <QDateTime>

#pragma execution_character_set("utf-8");
namespace Ui {
class goodsdelivery;
}

class goodsdelivery : public QWidget
{
    Q_OBJECT

private slots:
    void on_pushButton_OutputGoods_clicked();

    void on_pushButton_Exit_clicked();

public:
    explicit goodsdelivery(QWidget *parent = nullptr);
    ~goodsdelivery();

private:
    Ui::goodsdelivery *ui;
public:
    void InitComboBoxFunc(); // 初始化Combo box控件

};

#endif // GOODSDELIVERY_H
