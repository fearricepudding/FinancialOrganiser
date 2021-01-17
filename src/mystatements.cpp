#include "mystatements.h"
#include "../ui/ui_mystatements.h"
#include "database.h"
#include "mainwindow.h"

#include <iostream>

myStatements::myStatements(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::myStatements)
{
    ui->setupUi(this);

    database *db = database::instance();
    Json::Value statements = db->getState();
    QListWidget *list = ui->statemenntList;
    for (auto const& id : statements.getMemberNames()) {
        std::cout << id << std::endl;
        list->addItem(QString::fromStdString(id));
    }

    connect(ui->statemenntList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(statementClicked(QListWidgetItem*)));
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &myStatements::selectedSlot);
}

myStatements::~myStatements()
{
    delete ui;
}

void myStatements::selectedSlot(){
    emit newStatement(currentStatement);
}

void myStatements::statementClicked(QListWidgetItem *item){
    currentStatement = item->text().toStdString();
}
