#include "importstatement.h"
#include "../ui/ui_importstatement.h"
#include "CsvParser.h"
#include "database.h"

#include <QFileDialog>
#include <iostream>

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
    smt.loadStatement(statementName);
    db->save();
}

importStatement::~importStatement()
{
    delete ui;
}
