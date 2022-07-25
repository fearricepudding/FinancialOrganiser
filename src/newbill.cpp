#include "newbill.h"
#include "../ui/ui_newbill.h"
#include "database.h"
#include <string>
#include <list>
#include <iostream>

newbill::newbill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newbill)
{
    ui->setupUi(this);

    this->availableStatements = ui->availableStatements;
    this->selectedStatements = ui->selectedStatements;

    connect(ui->availableStatements, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(availableStatementSelected(QListWidgetItem*)));
    connect(ui->selectedStatements, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectedStatementSelected(QListWidgetItem*)));
    connect(ui->cancelButton, &QPushButton::released, this, &newbill::cancel);
    connect(ui->addStatement, &QPushButton::released, this, &newbill::addStatement);
    connect(ui->removeStatement, &QPushButton::released, this, &newbill::removeStatement);
    connect(ui->autoButton, &QPushButton::released, this, &newbill::autoCompare);
    connect(&selectbillWindow, SIGNAL(refreshBills()), this, SLOT(sendRefreshBills()));
}

newbill::~newbill()
{
    delete ui;
}

void newbill::cancel() {
    this->destroy();
}

// Check if the same ammount of single transactions occurs every month
void newbill::autoCompare() {
    QProgressBar* progress = ui->compareProgress;
    database* db = database::instance();
    std::list<Json::Value> statements; 
    Json::Value bills;
    int statementCount = ui->selectedStatements->count();
    dbg->out("Comparing " + std::to_string(statementCount) + " statements");
    int totalTrans = 0;

    for (int i = 0; i < statementCount; i++) {
        std::string statementName = ui->selectedStatements->item(i)->text().toStdString();
        Statement statement = db->getStatement(statementName);
        Json::Value transactions = statement.getTransactions();
        totalTrans += transactions.size();
        statements.push_back(transactions);
    }

    float progressStep = totalTrans / 100;

    Json::Value check;
    Json::Value track;
    bool first = true;
    float matchPrecision = 10.f; // XXX: Belongs in config
    for (Json::Value& current : statements) {
        for (int i = 0; i < current.size(); i++) {
            Json::Value transaction = current[i];
            const char* transID = transaction["Transaction Description"].asCString();
            float amount = std::atof(transaction["Debit Amount"].asCString());
            if (amount == NULL) {
                amount = std::atof(transaction["Credit Amount"].asCString());
            }
            //dbg->out(transID);
            if (first) {
                track[transID] = amount;
            } else {
                if (check.size() > 0) {
                    //std::cout << transID << std::endl;
                    if (check.isMember(transID)) {
                        if (check[transID].asFloat() == amount 
                            || (check[transID].asFloat() > amount-matchPrecision 
                                || check[transID].asFloat() < amount+matchPrecision)
                            ) {
                            track[transID] = amount;
                        }
                    }
                }
            }
            ui->compareProgress->setValue(ui->compareProgress->value() + progressStep);
        }
        //std::cout << "SIZE " << track.size() << std::endl;
        first = false;
        check.clear();
        check.copy(track);
        track.clear();
    }

   // std::cout << "================" << std::endl;
   // for (std::string name : check.getMemberNames()) {
   //     std::cout << name << " @ " << check[name].asFloat() << std::endl;
   // }

    this->selectbillWindow.populate(check);
    this->destroy();
}

void newbill::sendRefreshBills() {
    std::cout << "newbill REF" << std::endl;
    emit refreshBills();
}

void newbill::manualSelect() {

}

void newbill::availableStatementSelected(QListWidgetItem *item) {
    this->addSelectedItems = this->availableStatements->selectionModel()->selectedIndexes();
}

void newbill::selectedStatementSelected(QListWidgetItem* item) {
    this->removeSelectedItems = this->selectedStatements->selectionModel()->selectedIndexes();
}

void newbill::addStatement() {
    for (int i = 0; i < this->addSelectedItems.length(); i++) {
        dbg->out("Adding " + this->addSelectedItems.at(i).data().toString().toStdString() + " at index " + std::to_string(this->addSelectedItems.at(i).row()));
        QListWidgetItem* selected = this->availableStatements->takeItem(this->addSelectedItems.at(i).row());
        this->selectedStatements->addItem(selected);
    }
}

void newbill::removeStatement() {
    for (int i = 0; i < this->removeSelectedItems.length(); i++) {
        dbg->out("Removing " + this->addSelectedItems.at(i).data().toString().toStdString() + " at index " + std::to_string(this->addSelectedItems.at(i).row()));
        QListWidgetItem* selected = this->selectedStatements->takeItem(this->removeSelectedItems.at(i).row());
        this->availableStatements->addItem(selected);

    }
}

void newbill::populate() {
    dbg->out("Populating newbill");
    database* db = database::instance();
    Json::Value statements = db->getStatementNames();
    this->availableStatements->clear();
    this->selectedStatements->clear();
    for (int i = 0; i < statements.size(); i++) {
        std::string id = statements[i].asString();
        this->availableStatements->addItem(QString::fromStdString(id));
    };
}
