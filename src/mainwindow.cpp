#include <iostream>
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"



#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
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
    totalsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::createTableRow(QTableWidget *&table, const char *key, const char* value, int position){
    QTableWidgetItem *keyCell = new QTableWidgetItem();
    QTableWidgetItem *valueCell = new QTableWidgetItem();
    keyCell->setText(QString::fromUtf8(key));
    valueCell->setText(QString::fromUtf8(value));
    table->setItem(position, 0, keyCell);
    table->setItem(position, 1, valueCell);
};


void MainWindow::updateStatementItems(std::string statementName){
    dbg->out("Updating statement items...");
    Json::Value statement = db->getStatement(statementName);

    if(statement.size() <= 0){
        return;
    };

    QTableWidget *statementTable = ui->statementTable;
    QTableWidget *totalsTable = ui->statementTotals;

    QStringList titles;
    titles << "Reference" << "Ammount";
    statementTable->setHorizontalHeaderLabels(titles);
    statementTable->setColumnCount(2);
    statementTable->setRowCount(statement.size());
    statementTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    statementTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    dbg->out("Init totals table...");
    setTotalsTable();

    float monthTotal = 0.f;
    float monthBalance = 0.f;
    float totalIn = 0.f;
    float totalOut = 0.f;
    
    dbg->out("Looping statement...");
    for(int i = 0; i < statement.size(); i++){
        Json::Value item = statement[i];
        // Add item to list
        const char* title = "**Undefined**";
        if(!item["Transaction Description"].empty()){
            title = item["Transaction Description"].asCString();
        }

        const char* text = "";
        if(!item["Debit Amount"].empty()){
            std::string debit = item["Debit Amount"].asCString();
            if(debit.length() > 0){
				try{
					monthTotal += std::stof(debit);
					monthBalance -= std::stof(debit);
				}catch(const std::invalid_argument& ia){
					std::cout << "Failed to STOF 1" << std::endl;
				}
            };
            this->createTableRow(statementTable, title, text, i);
        };

		// Total in
        if(!item["Credit Amount"].empty()){
			std::string credit = item["Credit Amount"].asCString();
			if(credit.length() > 0)
				try{
					if(strlen(text) > 0){
						monthBalance += std::stof(credit);
						totalIn += std::stof(credit);
					};
				}catch(const std::invalid_argument& ia){
					std::cout << "Failed to convert value to float" << std::endl;
				};
			};
			this->createTableRow(statementTable, title, text, i);
        };

    dbg->out("Setting totals...");

    this->createTableRow(totalsTable, "Total Out", std::to_string(monthTotal).c_str(), 0);
    this->createTableRow(totalsTable, "Total In", std::to_string(totalIn).c_str(), 1);
    this->createTableRow(totalsTable, "Balance for statement", std::to_string(monthBalance).c_str(), 2);
    this->createTableRow(totalsTable, "Testing", "new", 3);

   // statementTable->adjustSize();


};

void MainWindow::openStatementImporter(){
    newStatement.show();
};
