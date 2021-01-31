#include <iostream>
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"



#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    //this->statusBar()->setSizeGripEnabled(false);

    //this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    //this->setFixedSize(QSize(770, 619));
    ui->setupUi(this);
    //updateStatementItems(selectedStateament);
    // Import and open statements
    connect(ui->newStatement, &QPushButton::released, this, &MainWindow::openStatementImporter);
    connect(&myStatementsWindow, SIGNAL(newStatement(std::string)), this, SLOT(changedStatement(std::string)));
    connect(ui->openStatements, &QPushButton::released, this, &MainWindow::openStatements);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::openStatements(){
    dbg->out("Opening statements");
    myStatementsWindow.updateList();
    myStatementsWindow.show();
}

void MainWindow::changedStatement(std::string name){
    dbg->out("New statement selected: "+name);
    selectedStateament = name;
    updateStatementItems(selectedStateament);
}

void MainWindow::setTotalsTable(){
    dbg->out("Setting up totals table");
    QTableWidget *totalsTable = ui->statementTotals;
    QStringList titles;
    titles << "Total" << "Price";
    totalsTable->setColumnCount(2);
    totalsTable->setRowCount(8);
    totalsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    totalsTable->setHorizontalHeaderLabels(titles);

    QTableWidgetItem *cell = new QTableWidgetItem();
    cell->setText("Total Bills: ");
    totalsTable->setItem(0, 0, cell);
}


void MainWindow::updateStatementItems(std::string statementName){
    dbg->out("Updating statement items...");
    Json::Value statement = db->getStatement(statementName);

    QTableWidget *statementTable = ui->statementTable;
    QTableWidget *totalsTable = ui->statementTotals;

    QStringList titles;
    titles << "Reference" << "Ammount";
    statementTable->setHorizontalHeaderLabels(titles);
    statementTable->setColumnCount(2);
    statementTable->setRowCount(statement.size());
    statementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    dbg->out("Init totals table...");
    setTotalsTable();

    if(statement.size() <= 0){
        return;
    };

    float monthTotal = 0;
    float monthBalance = 0;
    const char *text;
    QTableWidgetItem *cell = new QTableWidgetItem();

    dbg->out("Looping statement...");
    for(int i = 0; i < statement.size(); i++){
        Json::Value item = statement[i];
        // Add item to list
        if(!item["Transaction Description"].empty()){
            text = item["Transaction Description"].asCString();
            if(strlen(text) > 0){
                QTableWidgetItem *cell = new QTableWidgetItem(text);
                statementTable->setItem(i, 0, cell);
            };
        }

        if(!item["Debit Amount"].empty()){
            cell = new QTableWidgetItem();
            text = item["Debit Amount"].asCString();
            cell->setTextColor(Qt::red);
            if(strlen(text) <= 0){
                text = item["Credit Amount"].asCString();
                cell->setTextColor(Qt::green);
                if(strlen(text) <= 0){
                    text = "err";
                }
            };
            std::string debit = item["Debit Amount"].asString();
            if(debit.length() > 0){
                monthTotal += std::stof(debit);
                monthBalance -= std::stof(debit);
            };
            cell->setText(text);
            statementTable->setItem(i, 1, cell);
        }



        // Get total spent



        // Get credits
        if(!item["Credit Amount"].empty()){
            std::string credit = item["Credit Amount"].asString();
            if(credit.length() > 0){
                monthBalance += std::stof(credit);
            };
        }

    };

    dbg->out("Setting totals...");
    cell = new QTableWidgetItem();
    cell->setText(QString::fromStdString(std::to_string(monthTotal)));
    totalsTable->setItem(0, 1, cell);

   // statementTable->adjustSize();


};

void MainWindow::openStatementImporter(){
    newStatement.show();
};
