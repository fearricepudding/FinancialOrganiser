#include <iostream>
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"
#include "style.h"

#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->newStatement, &QPushButton::released, this, &MainWindow::openStatementImporter);
    connect(&myStatementsWindow, SIGNAL(newStatement(std::string)), this, SLOT(changedStatement(std::string)));
    connect(&newStatement, SIGNAL(newStatement(std::string)), this, SLOT(changedStatement(std::string)));
    connect(&newbillWindow, SIGNAL(refreshBills()), this, SLOT(refreshBills()));
    connect(ui->openStatements, &QPushButton::released, this, &MainWindow::openStatements);
    connect(ui->newbillButton, &QPushButton::released, this, &MainWindow::openNewbill);
    totalBills = 0.f;
    this->refreshBills();
}

MainWindow::~MainWindow()
{
    delete ui;
    this->destroy();
}

void MainWindow::openNewbill() {
    dbg->out("Openning new bill window");
    newbillWindow.populate();
    newbillWindow.show();
}

void MainWindow::openStatements()
{
    dbg->out("Opening statements");
    myStatementsWindow.updateList();
    myStatementsWindow.show();
}

void MainWindow::changedStatement(std::string name)
{
    dbg->out("New statement selected: " + name);
    selectedStateament = name;
    currentStatement = db->getStatement(selectedStateament);
    updateStatementItems();
    refreshBills();
}

void MainWindow::setupTable(QTableWidget *&table, QStringList &titles)
{
    table->clear();
    table->setRowCount(0);
    table->setColumnCount(titles.size());
    table->setHorizontalHeaderLabels(titles);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::createTableRow(QTableWidget *&table, const char *key, const char *value, int position)
{
    QBrush defaultColour = Ctransparent;
    createTableRow(table, key, value, position, defaultColour);
};

void MainWindow::createTableRow(QTableWidget *&table, const char *key, const char *value, int position, QBrush background){
    QTableWidgetItem *keyCell = new QTableWidgetItem();
    QTableWidgetItem *valueCell = new QTableWidgetItem();
    keyCell->setText(QString::fromUtf8(key));
    valueCell->setText(QString::fromUtf8(value));
    keyCell->setBackground(background);
    valueCell->setBackground(background);
    int rows = table->rowCount();
    table->setRowCount(rows+1);    
    table->setItem(position, 0, keyCell);
    table->setItem(position, 1, valueCell);
};

void MainWindow::updateStatementItems()
{
    dbg->out("Updating statement items...");
    Json::Value transactions = currentStatement.getTransactions();

    if (transactions.size() <= 0)
    {
        dbg->err("Statement size invalid");
        return;
    };

    QTableWidget *statementTable = ui->statementTable;
    QTableWidget *totalsTable = ui->statementTotals;

    dbg->out("Init tables...");

    QStringList titles;
    titles << "Total" << "Price";
    setupTable(totalsTable, titles);
    titles.clear();
    titles << "Reference" << "Amount";
    setupTable(statementTable, titles);

    float monthTotal = 0.f;
    float monthBalance = 0.f;
    float totalIn = 0.f;
    float totalOut = 0.f;

    dbg->out("Looping statement...");
    for (int i = 0; i < transactions.size(); i++)
    {
        bool isBill = false;
        Json::Value item = transactions[i];
        const char *title = "**Undefined**";
        if (!item["Transaction Description"].empty())
        {
            title = item["Transaction Description"].asCString();
            if(Bill::checkForReference(&this->bills, title)){
                isBill = true;
            };
        }

        std::string debit;
        if (!item["Debit Amount"].empty())
        {
            debit = item["Debit Amount"].asCString();
            if (strlen(debit.c_str()) > 0)
            {
                try
                {
                    monthTotal += std::stof(debit);
                    monthBalance -= std::stof(debit);
                }
                catch (const std::invalid_argument &ia)
                {
                    dbg->err("Failed to STOF debit");
                }
                std::string debitString = "-" + debit;
                if(isBill){
                    this->createTableRow(statementTable, title, debitString.c_str(), i, CMonBill);
                }else{
                    this->createTableRow(statementTable, title, debitString.c_str(), i, CMonOut);
                }
            };
        };

        std::string credit;
        if (!item["Credit Amount"].empty())
        {
            credit = item["Credit Amount"].asCString();
            if (strlen(credit.c_str()) > 0)
            {
                try
                {
                    monthBalance += std::stof(credit);
                    totalIn += std::stof(credit);
                }
                catch (const std::invalid_argument &ia)
                {
                    dbg->err("Failed to STOF credit");
                };
                this->createTableRow(statementTable, title, credit.c_str(), i, CMonIn);
            };
        };
    };
dbg->out("Setting totals...");

    this->createTableRow(totalsTable, "Total Out", std::to_string(monthTotal).c_str(), 0);
    this->createTableRow(totalsTable, "Total In", std::to_string(totalIn).c_str(), 1);
    this->createTableRow(totalsTable, "Bills total", std::to_string(this->totalBills).c_str(), 2);
    this->createTableRow(totalsTable, "After Bills", std::to_string(totalIn-this->totalBills).c_str(), 3);
    QBrush balanceBackground = Ctransparent;
    if(monthBalance < 0){
        balanceBackground = Cred;
    };
    this->createTableRow(totalsTable, "Balance for statement", std::to_string(monthBalance).c_str(), 4, balanceBackground);
    
};

void MainWindow::openStatementImporter()
{
    newStatement.show();
};

void MainWindow::refreshBills(){
    dbg->out("Refresh bills");
    QTableWidget *table = ui->billsTable;
    table->clear();
    std::vector<Bill> bills = db->getBills();
    this->bills = bills;
    QStringList titles;
    titles << "Reference"
        << "Ammount";
    setupTable(table, titles);

    bool hasTansactions = false;
    if(currentStatement.getTransactions().size() > 0){
        dbg->out("currentStatement has transactions");
        hasTansactions = true;
    }
    dbg->out(hasTansactions);
    dbg->out("refresh bills size", bills.size());
    for (int i = 0; i < bills.size(); i++){
        Bill bill = bills.at(i);
        std::string item = bill.getReference();
        std::string value = bill.getTotal();
        dbg->out(item);
        dbg->out(value);
        if(hasTansactions){
            if(currentStatement.hasTransaction(item)){
                this->createTableRow(table, item.c_str(), value.c_str(), i, Cgreen);   
            }else{
                this->createTableRow(table, item.c_str(), value.c_str(), i, Cred);
            }
        }else{
            this->createTableRow(table, item.c_str(), value.c_str(), i);
        }
        try {
            float valueF = std::stof(value);
            this->totalBills += valueF;
        }
        catch (const std::invalid_argument& ia) {
            dbg->err("Invalid argument");
        }
    }
}