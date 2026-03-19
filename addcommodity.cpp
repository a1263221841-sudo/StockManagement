#include "addcommodity.h"
#include "ui_addcommodity.h"

AddCommodity::AddCommodity(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCommodity)
{
    ui->setupUi(this);

    //禁止窗口最大化按扭
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);

    //禁止用于调整窗口大小
    setFixedSize(this->width(),this->height());

    //光标定位(焦点)
    ui->lineEdit_Id->setFocus();
}

AddCommodity::~AddCommodity()
{
    delete ui;
}
void AddCommodity::on_pushButton_Exit_clicked()
{
    close();
}

void AddCommodity::on_pushButton_Add_clicked()
{
    QSqlQuery sqlResult;
    QString StrId,StrName,StrAmount,StrUnitPrice,StrSupplier,StrDirector,StrInputTime,StrOutputTime,StrRemarks;

    StrId=ui->lineEdit_Id->text();
    StrName=ui->lineEdit_Name->text();
    StrAmount=ui->lineEdit_Amount->text();
    StrUnitPrice=ui->lineEdit_UnitPrice->text();
    StrSupplier=ui->lineEdit_Supplier->text();
    StrDirector=ui->lineEdit_Director->text();

    QDateTime datetime=QDateTime::currentDateTime();
    QString strnowinputtime=datetime.toString("yyyy-MM-dd HH:mm:ss");

        StrInputTime=strnowinputtime;
        StrOutputTime=strnowinputtime;

        StrRemarks=ui->textEdit_Remarks->toPlainText();

        if(ui->lineEdit_Id->text().isEmpty())
        {
            QMessageBox::critical(this,"Tips","新增商品编号不能为空,请重新输入?");
            ui->lineEdit_Id->setFocus();
            return;

        }
        QString sqlquery=QString("insert into stockdatatable(StockId,StockName,StockAmount,StockUnitPrice,Supplier,Director,WareHousTime,DeliveryTime,Remarks) "
                                    "values('%1','%2','%3','%4','%5','%6','%7','%8','%9')")
                   .arg(StrId).arg(StrName).arg(StrAmount).arg(StrUnitPrice).arg(StrSupplier).arg(StrDirector).arg(StrInputTime).arg(StrOutputTime).arg(StrRemarks);

           if(sqlResult.exec(sqlquery))
           {
               QMessageBox::information(this,"Tips","恭喜你，商品记录插入成功？");
           }
           else
           {
               qDebug() << "插入失败原因:" << sqlResult.lastError().text();
               QMessageBox::critical(this,"Tips","对不起商品记录插入失败，请重新检查？");
           }


}
