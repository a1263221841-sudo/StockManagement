#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

//操作数据库相关头文件
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QtDebug>

#include <QTableWidget>
#include <QTableWidget>
#include <QStringList>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;

public:
    void ConnectMySQLDBFunc();//连接MySQL数据库函数

    void TableWidgetListDataFunc(); //tableWidget表格控件初始化操作
    void InitTableWidgetFunc();//初始化表格控件列宽度

private slots:

    void on_pushButton_DataSearch_clicked();
     void on_pushButton_AddGoods_clicked();
     void on_pushButton_DeleteGoods_clicked();
     void on_pushButton_GoodsWareHousing_clicked();
      void on_pushButton_GoodsDelivery_clicked();
     void on_pushButton_LoadData_clicked();
        void on_pushButton_DataSummary_clicked();

};
#endif // MAINWIDGET_H
