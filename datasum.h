#ifndef DATASUM_H
#define DATASUM_H

#pragma execution_chararcter_set("utf-8")
#include <QWidget>

//表格控件相关的头文件
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>

//MySQL相关头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <qdebug.h>
#include <QAxObject>
#include <QDesktopServices>
#include <QDateTime>
#include <QDir>
#include <QInputDialog>
#include <QTableWidget>
#include <QTableWidget>

#include <QFileDialog>
namespace Ui {
class DataSum;
}

class DataSum : public QWidget
{
    Q_OBJECT

public:
    explicit DataSum(QWidget *parent = nullptr);
    ~DataSum();

private slots:
    void on_pushButton_DataSum_clicked();

    void on_pushButton_InputData_clicked();

    void on_pushButton_OutputData_clicked();

    void on_pushButton_DataBackups_clicked();

public:
    void TableWidgetListDataFunc();

private:
    Ui::DataSum *ui;
};

#endif // DATASUM_H
