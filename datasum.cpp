#include "datasum.h"
#include "ui_datasum.h"

DataSum::DataSum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSum)
{
    ui->setupUi(this);
    //禁用窗口最大化按扭
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);

    //禁止改变大小
    setFixedSize(this->width(),this->height());

    // 专门设计一个函数，初始化表格控件
        TableWidgetListDataFunc();

}

DataSum::~DataSum()
{
    delete ui;
}

//初始化表格控件
void DataSum::TableWidgetListDataFunc()
{
    //商品编号 商品名称 商品数量 商品单价 总价 供应商家 负责人 入库时间 出库时间 备注

    ui->tableWidget_ListData->setRowCount(200);
    ui->tableWidget_ListData->setColumnCount(10);

    ui->tableWidget_ListData->setFont(QFont("隶书",10));

    //设置表头
    ui->tableWidget_ListData->setHorizontalHeaderLabels(QStringList()<<"商品编号"<<"商品名称"<<"商品数量"<<"商品单价"<<"总价"
                                                        <<"供应商家"<<"负责人"<<"入库时间"<<"出库时间"<<"备注");
    ui->tableWidget_ListData->setSelectionBehavior(QAbstractItemView::SelectRows);  // 整行选中
    ui->tableWidget_ListData->setSelectionMode(QAbstractItemView::SingleSelection);  // 单选

}

void DataSum::on_pushButton_DataSum_clicked()
{


    ui->tableWidget_ListData->setColumnWidth(0,80);
    ui->tableWidget_ListData->setColumnWidth(1,100);
    ui->tableWidget_ListData->setColumnWidth(2,80);
    ui->tableWidget_ListData->setColumnWidth(3,80);
    ui->tableWidget_ListData->setColumnWidth(4,100);

    ui->tableWidget_ListData->setColumnWidth(5,300);
    ui->tableWidget_ListData->setColumnWidth(6,80);
    ui->tableWidget_ListData->setColumnWidth(7,200);
    ui->tableWidget_ListData->setColumnWidth(8,200);
    ui->tableWidget_ListData->setColumnWidth(9,200);

    int i =0;
    QSqlQuery sqlquery;
    sqlquery.exec("select * from stockdatatable");

    while (sqlquery.next()) {
        QString StrId=sqlquery.value(0).toString();
        QString StrName=sqlquery.value(1).toString();
        QString StrAmount=sqlquery.value(2).toString();
        QString StrUnitPrice=sqlquery.value(3).toString();

        //商品单价*商品数量=总价
//        double dSum = StrAmount.toDouble()*StrUnitPrice.toDouble();
//        QString StrSum=QString("%1").arg(dSum);
        // 计算总价（注意：若数据无效，给出0并可选警告）
               bool okAmount, okPrice;
               double amount = StrAmount.toDouble(&okAmount);
               double price = StrUnitPrice.toDouble(&okPrice);
               double dSum = (okAmount && okPrice) ? (amount * price) : 0.0;
               if (!okAmount || !okPrice) {
                   // 可记录日志或设置特殊标记
                   qDebug() << "数据无效：数量或单价非数字，商品ID：" << StrId;
               }
               QString StrSum = QString::number(dSum, 'f', 2);  // 保留两位小数

        QString StrSupplier=sqlquery.value(4).toString();
        QString StrDirector=sqlquery.value(5).toString();
        QString StrWarehousingTime=sqlquery.value(6).toString();
        QString StrDeliveryTime=sqlquery.value(7).toString();
        QString StrRemarks=sqlquery.value(8).toString();

             ui->tableWidget_ListData->setItem(i,0,new QTableWidgetItem(StrId));
               ui->tableWidget_ListData->setItem(i,1,new QTableWidgetItem(StrName));
               ui->tableWidget_ListData->setItem(i,2,new QTableWidgetItem(StrAmount));
               ui->tableWidget_ListData->setItem(i,3,new QTableWidgetItem(StrUnitPrice));

               // 显示商品总价
               ui->tableWidget_ListData->setItem(i,4,new QTableWidgetItem(StrSum));

               ui->tableWidget_ListData->setItem(i,5,new QTableWidgetItem(StrSupplier));
               ui->tableWidget_ListData->setItem(i,6,new QTableWidgetItem(StrDirector));
               ui->tableWidget_ListData->setItem(i,7,new QTableWidgetItem(StrWarehousingTime));
               ui->tableWidget_ListData->setItem(i,8,new QTableWidgetItem(StrDeliveryTime));
               ui->tableWidget_ListData->setItem(i,9,new QTableWidgetItem(StrRemarks));

               i++;
    }

}

void DataSum::on_pushButton_InputData_clicked()
{
    //获取选中行
    int row = ui->tableWidget_ListData->currentRow();
    if (row == -1)
    {
        QMessageBox::warning(this,"提示","请选中要入库的商品");
        return;
    }

    //获取商品编号
    QTableWidgetItem *idItem = ui->tableWidget_ListData->item(row,0);
//   qDebug() << "当前选中行：" << ui->tableWidget_ListData->currentRow();


//   // 调试：输出选中行的所有内容
//       qDebug() << "=== 选中行" << row << "的数据 ===";
//       for (int col = 0; col < ui->tableWidget_ListData->columnCount(); ++col) {
//           QTableWidgetItem *item = ui->tableWidget_ListData->item(row, col);
//           qDebug() << "列" << col << ":" << (item ? item->text() : "空");
//       }


    if (!idItem) {
            QMessageBox::critical(this, "错误", "无法获取商品编号！");
            return;
        }
        QString stockId = idItem->text();
//         qDebug() << "商品编号：" << stockId;
        QTableWidgetItem *amountItem = ui->tableWidget_ListData->item(row, 2);
            if (!amountItem) {
                QMessageBox::critical(this, "错误", "无法获取当前库存数量！");
                return;
            }
            bool ok;
            int currentAmount = amountItem->text().toInt(&ok);
            if (!ok) {
                QMessageBox::critical(this, "错误", "当前库存数量格式错误！");
                return;
            }
         // 输入入库数量
            int inputAmount = QInputDialog::getInt(this, "入库", "请输入入库数量：", 1, 1, 999999, 1, &ok);
                if (!ok) return;
         //计算新库存
          int newAmount = currentAmount + inputAmount;

          //更新数据库
          QSqlQuery query;
          query.prepare("update stockdatatable set StockAmount = :amount where StockId = :id");
          query.bindValue(":amount", newAmount);
          query.bindValue(":id",stockId);
          if (!query.exec()) {
                  QMessageBox::critical(this, "错误", "更新库存失败：" + query.lastError().text());
                  return;
              }

              QMessageBox::information(this, "成功", QString("商品 %1 入库 %2 件，新库存为 %3 件。")
                                       .arg(stockId).arg(inputAmount).arg(newAmount));

              // 6. 刷新表格显示（重新执行汇总查询）
              on_pushButton_DataSum_clicked();  // 或调用单独的刷新函数

}

void DataSum::on_pushButton_OutputData_clicked()
{
    //获取选中行
    int row = ui->tableWidget_ListData->currentRow();
    if (row == -1)
    {
        QMessageBox::warning(this,"提示","请选中要出库的商品");
        return;
    }

    //获取商品编号
    QTableWidgetItem *idItem = ui->tableWidget_ListData->item(row,0);
    if (!idItem) {
            QMessageBox::critical(this, "错误", "无法获取商品编号！");
            return;
        }
        QString stockId = idItem->text();
        QTableWidgetItem *amountItem = ui->tableWidget_ListData->item(row, 2);
            if (!amountItem) {
                QMessageBox::critical(this, "错误", "无法获取当前库存数量！");
                return;
            }
            bool ok;
            int currentAmount = amountItem->text().toInt(&ok);
            if (!ok) {
                QMessageBox::critical(this, "错误", "当前库存数量格式错误！");
                return;
            }
         // 输入入库数量
            int inputAmount = QInputDialog::getInt(this, "出库", "请输入入库数量：", 1, 1, 999999, 1, &ok);
                if (!ok) return;
         //计算新库存
          int newAmount = currentAmount - inputAmount;

          //更新数据库
          QSqlQuery query;
          query.prepare("update stockdatatable set StockAmount = :amount where StockId = :id");
          query.bindValue(":amount", newAmount);
          query.bindValue(":id",stockId);
          if (!query.exec()) {
                  QMessageBox::critical(this, "错误", "更新库存失败：" + query.lastError().text());
                  return;
              }

              QMessageBox::information(this, "成功", QString("商品 %1 出库 %2 件，新库存为 %3 件。")
                                       .arg(stockId).arg(inputAmount).arg(newAmount));

              // 6. 刷新表格显示（重新执行汇总查询）
              on_pushButton_DataSum_clicked();  // 或调用单独的刷新函数
}

void DataSum::on_pushButton_DataBackups_clicked()
{
    // 1. 生成默认文件名（基于当前时间）
        QString strTemp = QDateTime::currentDateTime().toString("yyyy-MM-dd-hhmmss");
        QString defaultName = QString("./%1_backup.xls").arg(strTemp);

        // 2. 弹出保存对话框
        QString strFileName = QFileDialog::getSaveFileName(this, tr("备份Excel文件"), defaultName, tr("Excel Files(*.xls)"));
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

        // 4. 设置 Excel 属性（后台运行，不弹出警告）
        excel->dynamicCall("SetVisible(bool)", false);
        excel->setProperty("DisplayAlerts", false);

        // 5. 创建工作簿和工作表
        QAxObject *workbooks = excel->querySubObject("Workbooks");
        if (!workbooks)
        {
            QMessageBox::warning(this, "错误", "无法获取工作簿集合。");
            excel->dynamicCall("Quit()");
            delete excel;
            return;
        }
        workbooks->dynamicCall("Add");

        QAxObject *workbook = excel->querySubObject("ActiveWorkbook");
        if (!workbook)
        {
            QMessageBox::warning(this, "错误", "无法获取活动工作簿。");
            excel->dynamicCall("Quit()");
            delete excel;
            return;
        }

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

        // 6. 设置行高（可选）
        QAxObject *rows = worksheet->querySubObject("Rows");
        if (rows)
            rows->setProperty("RowHeight", 25);

        // 7. 写入表头（假设表格已有表头）
        int colCount = ui->tableWidget_ListData->columnCount();
        for (int i = 1; i <= colCount; ++i)
        {
            QAxObject *cell = worksheet->querySubObject("Cells(int,int)", 1, i);
            if (cell)
            {
                QTableWidgetItem *headerItem = ui->tableWidget_ListData->horizontalHeaderItem(i-1);
                QString headerText = headerItem ? headerItem->text() : QString("列%1").arg(i);
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
                    QTableWidgetItem *item = ui->tableWidget_ListData->item(j-2, k-1);
                    QString text = item ? item->text() : "";
                    cell->dynamicCall("SetValue(const QString&)", text);
                }
            }
        }

        // 9. 保存并关闭
        workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(strFileName));
        workbook->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        delete excel;

        QMessageBox::information(this, "成功", "数据备份完成！\n文件保存在：" + strFileName);
}
