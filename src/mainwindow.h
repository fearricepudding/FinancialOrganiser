#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"
#include "debug.h"
#include "newbill.h"
#include <QListWidgetItem>
#include <QTableWidget>
#include "statement.h"
#include "bill.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    importStatement newStatement;
    myStatements myStatementsWindow;
    newbill newbillWindow;
    Statement currentStatement;
    std::string selectedStateament;

    database* db = database::instance();
    debug *dbg = new debug(true, 2);
    void createTableRow(QTableWidget *&table, const char*, const char*, int position);
    void createTableRow(QTableWidget *&talbe, const char*, const char*, int position, QBrush background);
    void setupTable(QTableWidget *&, QStringList &);
    void updateStatementItems();
    float totalBills;
    std::vector<Bill> bills;

public slots:
    void refreshBills();

private slots:
	void openStatementImporter();
   // void statementItemClicked(QListWidgetItem *item);
    void openStatements();
    void openNewbill();
    void newGroup();
    void editGroup();
    void changedStatement(std::string name);
};
#endif // MAINWINDOW_H
