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
    setFixedSize(this->height(),this->width());

    // 初始化Combo Box控件（读取数据库，商品数据表中的编号显示到此控件）
        InitComboBoxFunc();

}

goodsdelivery::~goodsdelivery()
{
    delete ui;
}
void goodsdelivery::on_pushButton_OutputGoods_clicked()
{

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

