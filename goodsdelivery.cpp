#include "goodsdelivery.h"
#include "ui_goodsdelivery.h"

goodsdelivery::goodsdelivery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::goodsdelivery)
{
    ui->setupUi(this);

    //禁止窗口最大化按扭
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);

    //禁止用户调整窗口大小
    //setFixedSize(this->height(),this->width());

    // 初始化Combo Box控件（读取数据库，商品数据表中的编号显示到此控件）
        InitComboBoxFunc();

}

goodsdelivery::~goodsdelivery()
{
    delete ui;
}
void goodsdelivery::on_pushButton_OutputGoods_clicked()
{
    //获取Combo Box 的值
    QString StrCBId=ui->comboBox_Id->currentText();

    //判断出库的商品是否为空
    if(ui->lineEdit_Amount->text().isEmpty())
    {
        QMessageBox::critical(this,"提示","商品出库的数量不能为空,请重新检查?");
        ui->lineEdit_Amount->setFocus();
        return;
    }

    //设计SQL查询语句
    //SELECT *FROM stockdatatable where StockId = 1001
    QSqlQuery sqlquery;
    QString strid="StockId =";
    strid +=StrCBId;

    QString str=QString("SELECT *FROM stockdatatable where %1").arg(strid);
    sqlquery.exec(str);

    //获取数据表中商品编号的对应数量
    int i= 0;
    QString strAmount;
    while (sqlquery.next()) {
        strAmount=sqlquery.value(2).toString();
    }

    //将输入数量+数量表当中的数量
    int inputamount=ui->lineEdit_Amount->text().toInt();//用户输入数量
    int tableamount=strAmount.toUInt();//将数据表里的数据转化为整型
    int isum=tableamount-inputamount;//实现相减

    //int 转换成QString
    QString strresult=QString::number(isum);

    //更新数据表中数量字段的值
    QString strdb=QString ("update stockdatatable set stockamount=%1 where %2").arg(strresult).arg(strid);
    QString strtime = QDateTime::currentDateTime().toString();
    if(sqlquery.exec(strdb))
    {
        // ---------- 新增：插入流水记录 ----------
            QSqlQuery logQuery;
            logQuery.prepare("INSERT INTO stock_transaction (stock_id, transaction_type, quantity, remark) "
                             "VALUES (:id, 'out', :qty, :remark)");
            logQuery.bindValue(":id", StrCBId.toInt());   // stock_id 是 INT 类型，需转换
            logQuery.bindValue(":qty", inputamount);
            logQuery.bindValue(":remark", "商品出库");
            if (!logQuery.exec()) {
                qDebug() << "记录流水失败：" << logQuery.lastError().text();
                // 可根据需要决定是否回滚，此处简单记录日志
            }
            // ------------------------------------

        QMessageBox::information(this,"提示","恭喜你,商品出库成功!");
        emit dataChanged();//发射信号

    }else{
        QMessageBox::information(this,"提示","对不起,商品出库失败,请重新检查?");
    }

}



void goodsdelivery::on_pushButton_Exit_clicked()
{
    close();
}

void goodsdelivery::InitComboBoxFunc() // 初始化Combo box控件
{
    int i=0;
    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT * FROM stockdatatable");

    QString StrId;

    while(sqlQuery.next())
    {
        StrId=sqlQuery.value(0).toString();
        ui->comboBox_Id->insertItem(i,StrId);
        i++;
    }

}

