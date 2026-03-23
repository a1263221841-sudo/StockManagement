#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //设置主窗口背景颜色
    QPalette plt;
    plt.setColor(QPalette::Window,QColor(120,200,100));
    this->setPalette(plt);

     ConnectMySQLDBFunc();  // 连接MySQL数据库

     //禁止窗口最大化按扭
     setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

     //禁止用户改变大小
     setFixedSize(this->width(),this->height());

     // tablewidget表格控件初始化操作
         TableWidgetListDataFunc();
         // 初始化表格控件列宽度
   InitTableWidgetFunc();


}

MainWidget::~MainWidget()
{
    delete ui;
}

 void MainWidget::ConnectMySQLDBFunc()//连接MySQL数据库函数
 {
     QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
     db.setHostName("127.0.0.1");
     db.setPort(3306);
     db.setDatabaseName("stockmsdb");
     db.setPassword("Mryang1113");

     bool bok=db.open();//打开数据库

     if(!bok)
     {
         QMessageBox::critical(this,"提示","MySQL数据库连接失败!!!");
     }

 }
 void MainWidget::on_pushButton_DataSearch_clicked()
 {// 表格控件初始化
     InitTableWidgetFunc();

     // 先清除表格控件数据
         ui->tableWidget_ListData->clear();

         QSqlQuery sqlquery;
//         QString strid="StockId=";
//            strid=strid+ui->lineEdit_GoodsNumber->text();

         sqlquery.prepare("select * from stockdatatable where StockId = ?");
         sqlquery.addBindValue(ui->lineEdit_GoodsNumber->text());
         sqlquery.exec();

         // SQL查询语句
//             QString str=QString("select *from commoditydatatable where %1").arg(strid);
//             sqlquery.exec(str);
        //设置表格控件标题(表头)
          ui->tableWidget_ListData->setHorizontalHeaderLabels(QStringList()<<"编号"<<"名称"<<"数量"<<"单价"<<"供应商家"<<"负责人"<<"入库时间"
                                                                 <<"出库时间"<<"备注");
          int i=0;
             while(sqlquery.next())
             {
                 // 将数据库里面的数据表读取出来转换成对应的字符串,赋给对应字符串变量
                 QString strId=sqlquery.value(0).toString();
                 QString strName=sqlquery.value(1).toString();
                 QString strAmoumt=sqlquery.value(2).toString();
                 QString strUnitPrice=sqlquery.value(3).toString();
                 QString strSupplier=sqlquery.value(4).toString();
                 QString strDirector=sqlquery.value(5).toString();
                 QString strWareHouseTime=sqlquery.value(6).toString();
                 QString strDeliveryTime=sqlquery.value(7).toString();
                 QString strRemarks=sqlquery.value(8).toString();

                 // 将字符串显示到表格控件当中
                 ui->tableWidget_ListData->setItem(i,0,new QTableWidgetItem(strId));
                 ui->tableWidget_ListData->setItem(i,1,new QTableWidgetItem(strName));
                 ui->tableWidget_ListData->setItem(i,2,new QTableWidgetItem(strAmoumt));
                 ui->tableWidget_ListData->setItem(i,3,new QTableWidgetItem(strUnitPrice));
                 ui->tableWidget_ListData->setItem(i,4,new QTableWidgetItem(strSupplier));
                 ui->tableWidget_ListData->setItem(i,5,new QTableWidgetItem(strDirector));
                 ui->tableWidget_ListData->setItem(i,6,new QTableWidgetItem(strWareHouseTime));
                 ui->tableWidget_ListData->setItem(i,7,new QTableWidgetItem(strDeliveryTime));
                 ui->tableWidget_ListData->setItem(i,8,new QTableWidgetItem(strRemarks));
             }

 }

 //调用:新增商品对话框
#include "addcommodity.h"

void MainWidget::on_pushButton_AddGoods_clicked()
  {
    AddCommodity *adddlg=new AddCommodity();
        adddlg->show();

  }
void MainWidget::on_pushButton_DeleteGoods_clicked()
  {

  }

//商品出库
#include "goodsdelivery.h"
void MainWidget::on_pushButton_GoodsDelivery_clicked()
  {
    goodsdelivery *adddlg=new goodsdelivery();
    adddlg->show();
  }
//商品入库
#include "goodswarehousing.h"
void MainWidget::on_pushButton_GoodsWareHousing_clicked()
   {
    goodswarehousing *adddlg =new goodswarehousing();
    adddlg->show();
   }
  void MainWidget::on_pushButton_LoadData_clicked()
  {

  }
  void MainWidget::on_pushButton_DataSummary_clicked()
     {

     }


 void MainWidget::TableWidgetListDataFunc() //tableWidget表格控件初始化操作
 {
    //编号 名称 数量 单价 供应商家 负责人 入库时间 出库时间  备注
     //设置表格控件为200行9列

     ui->tableWidget_ListData->setColumnCount(9);
     ui->tableWidget_ListData->setRowCount(200);

     //设置表格控件字体大小和样式
     ui->tableWidget_ListData->setFont(QFont("隶书",13));

     //设置表格控件标题(表头)
     ui->tableWidget_ListData->setHorizontalHeaderLabels(QStringList()<<"编号"<<"名称"<<"数量"<<"单价"<<"供应商家"<<"负责人"<<"入库时间"
                                                         <<"出库时间"<<"备注");

 }
 void MainWidget::InitTableWidgetFunc()//初始化表格控件列宽度
 {
     ui->tableWidget_ListData->setColumnWidth(0,80);
        ui->tableWidget_ListData->setColumnWidth(1,150);
        ui->tableWidget_ListData->setColumnWidth(2,80);
        ui->tableWidget_ListData->setColumnWidth(3,80);
        ui->tableWidget_ListData->setColumnWidth(4,300);
        ui->tableWidget_ListData->setColumnWidth(5,80);
        ui->tableWidget_ListData->setColumnWidth(6,300);
        ui->tableWidget_ListData->setColumnWidth(7,300);
        ui->tableWidget_ListData->setColumnWidth(8,180);

        //SQL查询语句
        QSqlQuery sqlquery;
        sqlquery.exec("select *from stockdatatable");

        int i=0;
            while(sqlquery.next())
            {
                // 将数据库里面的数据表读取出来转换成对应的字符串,赋给对应字符串变量
                QString strId=sqlquery.value(0).toString();
                QString strName=sqlquery.value(1).toString();
                QString strAmoumt=sqlquery.value(2).toString();
                QString strUnitPrice=sqlquery.value(3).toString();
                QString strSupplier=sqlquery.value(4).toString();
                QString strDirector=sqlquery.value(5).toString();
                QString strWareHouseTime=sqlquery.value(6).toString();
                QString strDeliveryTime=sqlquery.value(7).toString();
                QString strRemarks=sqlquery.value(8).toString();

                // 将字符串显示到表格控件当中
                ui->tableWidget_ListData->setItem(i,0,new QTableWidgetItem(strId));
                ui->tableWidget_ListData->setItem(i,1,new QTableWidgetItem(strName));
                ui->tableWidget_ListData->setItem(i,2,new QTableWidgetItem(strAmoumt));
                ui->tableWidget_ListData->setItem(i,3,new QTableWidgetItem(strUnitPrice));
                ui->tableWidget_ListData->setItem(i,4,new QTableWidgetItem(strSupplier));
                ui->tableWidget_ListData->setItem(i,5,new QTableWidgetItem(strDirector));
                ui->tableWidget_ListData->setItem(i,6,new QTableWidgetItem(strWareHouseTime));
                ui->tableWidget_ListData->setItem(i,7,new QTableWidgetItem(strDeliveryTime));
                ui->tableWidget_ListData->setItem(i,8,new QTableWidgetItem(strRemarks));

                i++;
            }

 }



