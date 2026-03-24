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
    //连接信号
    connect(adddlg,&AddCommodity::dataChanged,this,&MainWidget::InitTableWidgetFunc);
        adddlg->show();

  }
void MainWidget::on_pushButton_DeleteGoods_clicked()
  {
    //提醒用户是否确认删除此记录
    int iClick=QMessageBox::warning(this,"警告","是否按商品编号进行删除",QMessageBox::Yes|QMessageBox::No);

    //判断用户是否点击Yes
    if(iClick==QMessageBox::Yes){
        //获取当前行进行删除
        int iRow =ui->tableWidget_ListData->currentRow();
        //获取行列的商品编码
        QString strValue=ui->tableWidget_ListData->model()->index(iRow,0).data().toString();

        QSqlQuery result;
        QString strid=strValue;

        //判空
        if(strid==NULL)
        {
            QMessageBox::critical(this,"错误","提示:请选择要删除的商品编号,请重新检查?");
            return;
        }

        //实现删除
        QString sqlquery =QString("delete from stockdatatable where StockId = %1").arg(strid);
        if(result.exec(sqlquery))
        {
            //删除提示
            QMessageBox::information(this,"提示","删除商品记录成功");

            //清空表格控件数据
            ui->tableWidget_ListData->clear();

            //设置表格控件控件标题(表头)
            ui->tableWidget_ListData->setHorizontalHeaderLabels(QStringList()<<"编号"<<"名称"<<"数量"<<"单价"<<"供应商家"<<"负责人"<<"入库时间"<<"出库时间"<<"备注");

            // 调用函数将数据表中数据再次展示到表格控件当中
                      InitTableWidgetFunc();
        }
    }else
    {
        return;
    }
  }

//商品出库
#include "goodsdelivery.h"
void MainWidget::on_pushButton_GoodsDelivery_clicked()
  {
    goodsdelivery *gwdlg=new goodsdelivery();
    // 连接信号：子窗口数据变化 -> 主窗口刷新表格
        connect(gwdlg, &goodsdelivery::dataChanged, this, &MainWidget::InitTableWidgetFunc);
    gwdlg->show();
  }
//商品入库
#include "goodswarehousing.h"
void MainWidget::on_pushButton_GoodsWareHousing_clicked()
   {
    goodswarehousing *gddlg =new goodswarehousing();
    // 连接信号：子窗口数据变化 -> 主窗口刷新表格
        connect(gddlg, &goodswarehousing::dataChanged, this, &MainWidget::InitTableWidgetFunc);
    gddlg->show();
   }

  //导出数据
#include <QFileDialog>
#include <QAxObject>
#include <QDesktopServices>
//导出表格控件数据,直接生成Excel文件保存
  void MainWidget::on_pushButton_LoadData_clicked()
  {
//    //保存文件扩展名为:.xls
//      QDateTime time;
//      QString strTemp;

//      //获取当前系统时间,作为文件名称进行保存
//      time =QDateTime::currentDateTime();
//      strTemp =time.toString("yyyy-MM-dd-hhmmss");

//      //应用文件对话框来保存要导出的文件名称(设置保存的文件名称)及数据信息
//      QString strFileName=QFileDialog::getSaveFileName(this,tr("Excel Files"),QString("./%1%2.xls").arg(strTemp).arg("_kcgl"),tr("Excel Files(*.xls)"));

//      //处理工作蒲
//      if(!(strFileName.isEmpty()))
//      {
//          QAxObject *excel=new QAxObject;
//          if(excel->setControl("Excel.Application"))
//          {
//            excel->dynamicCall("SetVisible (bool Visible)",false);//让 Excel 在后台运行，用户看不到界面。
//            excel->setProperty("DisplayAlerts",false);//关闭警告提示(如覆盖文件确认),避免干扰

//            QAxObject *workbooks =excel->querySubObject("WorkBooks");// 获得工作簿集合
//            workbooks->dynamicCall("Add"); // 创建一个工作簿
//            QAxObject *workbook =excel->querySubObject("ActiveWorkBook");// 获得当前工作簿
//            QAxObject *worksheet = workbook->querySubObject("Worksheets(int)",1);//获取第一个工作表（索引从 1 开始）。
//            QAxObject *cell;

//            //写入表头数据
//            for(int i = 1; i <= ui->tableWidget_ListData->columnCount(); i++)
//            {
//                cell = worksheet->querySubObject("Cells(int,int)", 1, i);
//                cell->setProperty("RowHeight", 25);
//                cell->dynamicCall("SetValue(const QString&)", ui->tableWidget_ListData->horizontalHeaderItem(i-1)->text());
//            }
//            // 2：将表格数据保存到Excel文件当中
//                      for(int j=2;j<=ui->tableWidget_ListData->rowCount()+1;j++)
//                      {
//                          for(int k=1;k<=ui->tableWidget_ListData->columnCount();k++)
//                          {
//                              cell=worksheet->querySubObject("Cells(int,int)",j,k);
//                              cell->dynamicCall("SetValue(const QString&)",ui->tableWidget_ListData->item(j-2,k-1)->text()+"\t");

//                          }
//                      }
//                      // 3：将刚才创建的Excel文件直接保存到指定的目录下
//                                 workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(strFileName)); // 保存到strFileName
//                                 workbook->dynamicCall("Close()");
//                                 excel->dynamicCall("Quit()");
//                                 delete excel;
//                                 excel=NULL;

//          }

//      }
      // 1. 生成默认文件名（注意路径写法）
         QString strTemp = QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmmss");
         QString defaultName = QString("./%1_kcgl.xls").arg(strTemp);  // 添加斜杠

         // 2. 弹出文件保存对话框
         QString strFileName = QFileDialog::getSaveFileName(this, tr("Excel Files"), defaultName, tr("Excel Files(*.xls)"));
         if (strFileName.isEmpty())
             return;

         // 3. 创建 Excel 对象
         QAxObject *excel = new QAxObject;
         if (!excel->setControl("Excel.Application"))
         {
             QMessageBox::warning(this, "错误", "无法启动 Excel，请确保已安装 Microsoft Excel。");
             delete excel;
             return;
         }

         // 设置 Excel 属性（注意参数写法：去掉多余空格）
         excel->dynamicCall("SetVisible(bool)", false);
         excel->setProperty("DisplayAlerts", false);

         // 4. 创建工作簿
         QAxObject *workbooks = excel->querySubObject("Workbooks");
         if (!workbooks)
         {
             QMessageBox::warning(this, "错误", "无法获取工作簿集合。");
             excel->dynamicCall("Quit()");
             delete excel;
             return;
         }
         workbooks->dynamicCall("Add");

         QAxObject *workbook = excel->querySubObject("ActiveWorkbook");  // 注意：Workbook 首字母大写
         if (!workbook)
         {
             QMessageBox::warning(this, "错误", "无法获取活动工作簿。");
             excel->dynamicCall("Quit()");
             delete excel;
             return;
         }

         // 5. 获取第一个工作表（正确方式）
         QAxObject *worksheets = workbook->querySubObject("Worksheets");
         if (!worksheets)
         {
             QMessageBox::warning(this, "错误", "无法获取工作表集合。");
             workbook->dynamicCall("Close()");
             excel->dynamicCall("Quit()");
             delete excel;
             return;
         }
         QAxObject *worksheet = worksheets->querySubObject("Item(int)", 1);
         if (!worksheet)
         {
             QMessageBox::warning(this, "错误", "无法获取工作表对象。");
             workbook->dynamicCall("Close()");
             excel->dynamicCall("Quit()");
             delete excel;
             return;
         }

         // 6. 设置行高（只设置一次）
         QAxObject *rows = worksheet->querySubObject("Rows");
         if (rows)
             rows->setProperty("RowHeight", 25);

         // 7. 写入表头
         int colCount = ui->tableWidget_ListData->columnCount();
         for (int i = 1; i <= colCount; ++i)
         {
             QAxObject *cell = worksheet->querySubObject("Cells(int,int)", 1, i);
             if (cell)
             {
                 QTableWidgetItem *headerItem = ui->tableWidget_ListData->horizontalHeaderItem(i - 1);
                 QString headerText = headerItem ? headerItem->text() : "";
                 cell->dynamicCall("SetValue(const QString&)", headerText);
             }
         }

         // 8. 写入数据
         int rowCount = ui->tableWidget_ListData->rowCount();
         for (int j = 2; j <= rowCount + 1; ++j)
         {
             for (int k = 1; k <= colCount; ++k)
             {
                 QAxObject *cell = worksheet->querySubObject("Cells(int,int)", j, k);
                 if (cell)
                 {
                     QTableWidgetItem *item = ui->tableWidget_ListData->item(j - 2, k - 1);
                     QString text = item ? item->text() : "";
                     // 注意：正确的方法名和参数格式
                     cell->dynamicCall("SetValue(const QString&)", text);
                 }
             }
         }

         // 9. 保存并退出
         workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(strFileName));
         workbook->dynamicCall("Close()");
         excel->dynamicCall("Quit()");
         delete excel;
  }

  //数据汇总,备份数据
#include "datasum.h"
  void MainWidget::on_pushButton_DataSummary_clicked()
     {
      DataSum *dsdlg =new DataSum();

      dsdlg->show();
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



