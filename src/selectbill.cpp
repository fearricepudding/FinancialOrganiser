#include "selectbill.h"
#include "../ui/ui_selectbill.h"
#include <qlistwidget.h>
#include "database.h"

selectbill::selectbill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectbill)
{
    ui->setupUi(this);

    this->table = ui->billList;

    QStringList titles;
    titles << "Reference"
        << "Ammount";
    this->table->setColumnCount(2);
    this->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->table->setHorizontalHeaderLabels(titles);
    this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->addBill, &QPushButton::released, this, &selectbill::newBills);
    connect(ui->cancel, &QPushButton::released, this, &selectbill::cancel);
}

selectbill::~selectbill()
{
    delete ui;
}

void selectbill::populate(Json::Value list) {
    this->table->setRowCount(list.size());
    int position = 0;
    for (std::string item : list.getMemberNames()) {
        QString itemText(item.c_str());
        Json::Value value = list[item];
        QString itemValue(std::to_string(value.asFloat()).c_str());
        this->addBillRow(itemText, itemValue, position);
        position++;
    }
    this->show();
}

void selectbill::addBillRow(QString id, QString price, int position) {
    QTableWidgetItem* keyCell = new QTableWidgetItem();
    QTableWidgetItem* valueCell = new QTableWidgetItem();
    keyCell->setText(id);
    valueCell->setText(price);
    this->table->setItem(position, 0, keyCell);
    this->table->setItem(position, 1, valueCell);
}

void selectbill::newBills() {
    QList<QTableWidgetItem*> selected = ui->billList->selectedItems();
    std::string desc;
    std::string value;
    Json::Value bills;
    int itt = 0;
    for (QTableWidgetItem *item : selected) {
        std::string text = item->text().toStdString();
        std::cout << text << std::endl;
        if (itt % 2 == 0) {
            desc = text;
        } else {
            value = text;
        }
        bills[desc] = value;
        itt++;
    }
    database* db = database::instance();
    db->addBills(bills);
    emit refreshBills();
}

void selectbill::cancel() {
    this->destroy();
}