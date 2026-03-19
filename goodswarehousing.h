#ifndef GOODSWAREHOUSING_H
#define GOODSWAREHOUSING_H

#include <QWidget>

namespace Ui {
class goodswarehousing;
}

class goodswarehousing : public QWidget
{
    Q_OBJECT

public:
    explicit goodswarehousing(QWidget *parent = nullptr);
    ~goodswarehousing();

private:
    Ui::goodswarehousing *ui;
};

#endif // GOODSWAREHOUSING_H
