#include "importstatement.h"
#include "../ui/ui_importstatement.h"

#include "database.h"

#include <cppcsv/include/CsvParser.h>
#include <QFileDialog>
#include <iostream>
#include <json/json.h>

importStatement::importStatement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importStatement)
{
    ui->setupUi(this);
    QPushButton *findBtn = ui->findBtn;
    connect(ui->btnbox, &QDialogButtonBox::accepted, this, &importStatement::submitted);
    connect(findBtn, &QPushButton::released, this, &importStatement::findStatement);
}

void importStatement::findStatement(){
    QString fileName = QFileDialog::getOpenFileName(this,
                            tr("Open Statement"));
    QLineEdit *statementPath = ui->statementPath;
    statementPath->setText(fileName);
}

void importStatement::submitted(){
    std::string statementPath = ui->statementPath->text().toStdString();
    std::string statementName = ui->statementName->text().toStdString();
    database *db = database::instance();
    CsvParser smt(statementPath);
    Json::Value statement = smt.loadStatement(statementName);
    db->addStatement(statement, statementName);
    db->save();
}

importStatement::~importStatement()
{
    delete ui;
}
