#include <iostream>
#include "mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"

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
    updateStatementItems(selectedStateament);
}

void MainWindow::setupTable(QTableWidget *&table, QStringList &titles)
{
    table->setColumnCount(titles.size());
    table->setHorizontalHeaderLabels(titles);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::createTableRow(QTableWidget *&table, const char *key, const char *value, int position)
{
    QTableWidgetItem *keyCell = new QTableWidgetItem();
    QTableWidgetItem *valueCell = new QTableWidgetItem();
    keyCell->setText(QString::fromUtf8(key));
    valueCell->setText(QString::fromUtf8(value));
    int rows = table->rowCount();
    table->setRowCount(rows+1);
    table->setItem(position, 0, keyCell);
    table->setItem(position, 1, valueCell);
};

void MainWindow::createTableRow(QTableWidget *&table, const char *key, const char *value, int position, QBrush background){
    createTableRow(table, key, value, position);
    table->itemAt(position, 1)->setBackground(Qt::cyan);
};

void MainWindow::updateStatementItems(std::string statementName)
{
    dbg->out("Updating statement items...");
    Statement statement = db->getStatement(statementName);
    Json::Value transactions = statement.getTransactions();

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
            if(this->bills.isMember(title)){
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
                this->createTableRow(statementTable, title, debitString.c_str(), i);
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
                this->createTableRow(statementTable, title, credit.c_str(), i);
            };
        };
    };
    dbg->out("Setting totals...");

    this->createTableRow(totalsTable, "Total Out", std::to_string(monthTotal).c_str(), 0);
    this->createTableRow(totalsTable, "Total In", std::to_string(totalIn).c_str(), 1);
    this->createTableRow(totalsTable, "Balance for statement", std::to_string(monthBalance).c_str(), 2);
    this->createTableRow(totalsTable, "Bills total", std::to_string(this->totalBills).c_str(), 3);
};

void MainWindow::openStatementImporter()
{
    newStatement.show();
};

void MainWindow::refreshBills(){
    dbg->out("Refresh bills");
    QTableWidget *table = ui->billsTable;
    table->clear();
    this->bills = db->getBills();
    QStringList titles;
    titles << "Reference"
        << "Ammount";
    table->setColumnCount(2);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setHorizontalHeaderLabels(titles);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setRowCount(this->bills.size());
    int itt = 0;
    for (std::string item : this->bills.getMemberNames()) {
        std::string value = this->bills[item].asString();
        this->createTableRow(table, item.c_str(), value.c_str(), itt);
        try {
            float valueF = std::stof(value);
            this->totalBills += valueF;
        }
        catch (const std::invalid_argument& ia) {
            dbg->err("Invalid argument");
        }
        itt++;
    }
}