#include "goodswarehousing.h"
#include "ui_goodswarehousing.h"

goodswarehousing::goodswarehousing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::goodswarehousing)
{
    ui->setupUi(this);
}

goodswarehousing::~goodswarehousing()
{
    delete ui;
}
