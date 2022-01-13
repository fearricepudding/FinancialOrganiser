#ifndef NEWBILL_H
#define NEWBILL_H

#include <QDialog>
#include "debug.h"
#include <qlistwidget.h>
#include "selectbill.h"

namespace Ui {
    class newbill;
}

class newbill : public QDialog
{
    Q_OBJECT

public:
    explicit newbill(QWidget* parent = nullptr);
    ~newbill();
    void populate();

private:
    Ui::newbill* ui;
    debug* dbg = new debug(true, 2);
    QListWidget* availableStatements;
    QListWidget* selectedStatements;
    QModelIndexList addSelectedItems;
    QModelIndexList removeSelectedItems;
    selectbill selectbillWindow;

signals:
    void refreshBills();

public slots:
    void sendRefreshBills();

private slots:
    void selectedStatementSelected(QListWidgetItem*);
    void availableStatementSelected(QListWidgetItem*);
    void addStatement();
    void removeStatement();
    void autoCompare();
    void manualSelect();
    void cancel();

};
#endif // NEWBILL_H