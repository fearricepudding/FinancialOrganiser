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
    updateStatementItems(selectedStateament);
    // Import and open statements
    connect(ui->newStatement, &QPushButton::released, this, &MainWindow::openStatementImporter);
    connect(&myStatementsWindow, SIGNAL(newStatement(std::string)), this, SLOT(changedStatement(std::string)));
    connect(ui->openStatements, &QPushButton::released, this, &MainWindow::openStatements);

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::openStatements(){
    myStatementsWindow.updateList();
    myStatementsWindow.show();
}

void MainWindow::changedStatement(std::string name){
    selectedStateament = name;
    updateStatementItems(selectedStateament);
}


void MainWindow::updateStatementItems(std::string statementName){
    Json::Value statement = db->getStatement(statementName);

    QTableWidget *table = ui->statementTable;

    table->setColumnCount(8);
    table->setRowCount(statement.size());
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList titles;

    for (auto const& id : statement[0].getMemberNames()) {
        std::cout << id << std::endl;
        QString tmp = QString::fromStdString(id);
        titles.append(tmp);std::cout << id << std::endl;
    }

    table->setHorizontalHeaderLabels(titles);

    if(statement.size() <= 0){
        return;
    };

    float monthTotal = 0;
    float monthBalance = 0;
    /*
    for(int i = 0; i < statement.size(); i++){
        Json::Value item = statement[i];
        // Add item to list

        // Get total spent
        std::string debit = item["Debit Amount"].asString();
        if(debit.length() > 0){
            monthTotal += std::stof(debit);
            monthBalance -= std::stof(debit);
        };

        // Get credits
        std::string credit = item["Credit Amount"].asString();
        if(credit.length() > 0){
            monthBalance += std::stof(credit);
        };

    };
    std::cout << "Total: " << monthBalance << std::endl;
    // Set total
    QString monthTotalQ = QString::number(monthTotal);
    // Set balance
    QString monthBalanceQ = QString::number(monthBalance);
    //set Start & end bal
    std::string startBal = statement[0]["Balance"].asString();
    std::string endBal = statement[statement.size()-1]["Balance"].asString();
   // float diff = std::stof(endBal) - std::stof(startBal);
   */

};

void MainWindow::openStatementImporter(){
    newStatement.show();
};
