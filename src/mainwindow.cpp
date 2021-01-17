#include <iostream>
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "FinancialOrganiser.h"
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateStatementItems(selectedStateament);
    connect(ui->newStatement, &QPushButton::released, this, &MainWindow::openStatementImporter);
    connect(ui->statementItems, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(statementItemClicked(QListWidgetItem*)));
    connect(ui->openStatements, &QPushButton::released, this, &MainWindow::openStatements);
    connect(&myStatementsWindow, SIGNAL(newStatement(std::string)), this, SLOT(changedStatement(std::string)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openStatements(){
    myStatementsWindow.show();
}

void MainWindow::changedStatement(std::string name){
    selectedStateament = name;
    std::cout << name << std::endl;
    updateStatementItems(selectedStateament);
}

void MainWindow::statementItemClicked(QListWidgetItem *item){
    QListWidget *list = ui->statementItems;
    int row = list->row(item);
    Json::Value statement = db->getStatement(selectedStateament);
    Json::Value itemsData = statement[row];
    // set item name
    ui->itemName->setText(itemsData["Transaction Description"].asCString());
    // Set value
    std::string credit = itemsData["Credit Amount"].asString();
    std::string debit = itemsData["Debit Amount"].asString();
    if(credit.length() > 0){
        ui->itemValue->setText("+ £"+QString::fromStdString(credit));
    }else if(debit.length() > 0){
        ui->itemValue->setText("- £"+QString::fromStdString(debit));
    };
    // Set date
    std::string date = itemsData["Transaction Date"].asString();
    ui->itemDate->setText(QString::fromStdString(date));
}

void MainWindow::updateStatementItems(std::string statementName){
    Json::Value statement = db->getStatement(statementName);
    QListWidget *list = ui->statementItems;
    list->clear();
    if(statement.size() <= 0){
        std::cout << statement.size() << std::endl;
        return;
    }
    QLabel *monthTotalLabel = ui->monthTotal;
    QLabel *monthBalanceLabel = ui->monthBalance;
    float monthTotal = 0;
    float monthBalance = 0;
    //std::cout << statement.size() << std::endl;
    for(int i = 0; i < statement.size(); i++){
        Json::Value item = statement[i];
        // Add item to list
        list->addItem(item["Transaction Description"].asCString());
        // Get total spent
        std::string debit = item["Debit Amount"].asString();
        if(debit.length() > 0){
            monthTotal += std::stof(debit);
            monthBalance -= std::stof(debit);
        }
        // Get credits
        std::string credit = item["Credit Amount"].asString();
        if(credit.length() > 0){
            monthBalance += std::stof(credit);
        }
    }
    std::cout << "Total: " << monthBalance << std::endl;
    // Set total
    QString monthTotalQ = QString::number(monthTotal);
    monthTotalLabel->setText("£"+monthTotalQ);
    // Set balance
    QString monthBalanceQ = QString::number(monthBalance);
    monthBalanceLabel->setText("£"+monthBalanceQ);

    //set Start & end bal
    std::string startBal = statement[0]["Balance"].asString();
    std::string endBal = statement[statement.size()-1]["Balance"].asString();
    float diff = std::stof(endBal) - std::stof(startBal);
    ui->balStart->setText("£"+QString::fromStdString(startBal));
    ui->balEnd->setText("£"+QString::fromStdString(endBal));
    ui->balDiff->setText("£"+QString::number(diff));
}

void MainWindow::openStatementImporter(){
    newStatement.show();
}

