#include "goodswarehousing.h"
#include "ui_goodswarehousing.h"

goodswarehousing::goodswarehousing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::goodswarehousing)
{

    ui->setupUi(this);
    //禁止窗口最大化按扭
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);

    //禁止用户调整窗口大小
    //setFixedSize(this->height(),this->width());

    //初始化Combo Box控件
    InitComboBoxFunc();
}

goodswarehousing::~goodswarehousing()
{
    delete ui;
}

void goodswarehousing::on_pushButton_InputGoods_clicked()
{
    QString StrCBId=ui->comboBox_Id->currentText();

    //判断商品入库是否为空
    if(ui->lineEdit_Amount->text().isEmpty())
    {
        QMessageBox::critical(this,"提示","商品入库数量不能为空,请重新检查?");
        ui->lineEdit_Amount->setFocus();
        return;
    }

    QSqlQuery sqlquery;
    QString strid="StockId=";
    strid+=StrCBId;

    QString str=QString("SELECT * FROM stockdatatable where %1").arg(strid);
    sqlquery.exec(str);

    int i=0;
    QString strAmount;
    while (sqlquery.next()) {
        strAmount=sqlquery.value(2).toString();
    }

    //相加
    int inputamount=ui->lineEdit_Amount->text().toInt();
    int tableamount=strAmount.toInt();
    int isum = inputamount+tableamount;

    //转换
    QString strresult=QString::number(isum);

    //更新
    QString strdb=QString("update stockdatatable set stockamount =%1 where %2").arg(strresult).arg(strid);

    if(sqlquery.exec(strdb))
    {
        QMessageBox::information(this,"提示","恭喜,入库成功");
    }else{
        QMessageBox::critical(this,"提示","对不起商品入库失败,请重新检查");
    }
}
void goodswarehousing::on_pushButton_Exit_clicked()
{
    close();
}
void goodswarehousing::InitComboBoxFunc()
{
    int i =0;
    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT * FROM stockdatatable");

    QString StrId;
    while (sqlQuery.next()) {
        StrId=sqlQuery.value(0).toString();
        ui->comboBox_Id->insertItem(i,StrId);
        i++;
    }

}
