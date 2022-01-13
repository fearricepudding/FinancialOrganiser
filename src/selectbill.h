#ifndef SELECTBILL_H
#define SELECTBILL_H

#include <QDialog>
#include "debug.h"
#include <json/json.h>
#include <qtablewidget>
#include "debug.h"

namespace Ui {
class selectbill;
}

class selectbill : public QDialog
{
    Q_OBJECT

public:
    explicit selectbill(QWidget *parent = nullptr);
    ~selectbill();

private:
    Ui::selectbill *ui;
    void addBillRow(QString, QString, int);
    QTableWidget* table;
    debug* dbg = new debug(true, 2);
    void cancel();

signals:
    void refreshBills();

public slots:
    void populate(Json::Value);
    void newBills();
};

#endif // SELECTBILL_H
