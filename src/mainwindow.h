#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"
#include "debug.h"
#include <QListWidgetItem>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateStatementItems(std::string statementName);

private:
    Ui::MainWindow *ui;
    importStatement newStatement;
    myStatements myStatementsWindow;
    database* db = database::instance();
    std::string selectedStateament;
    debug *dbg = new debug(true, 2);
    void clearStats();
    void createTableRow(QTableWidget *&table, const char*, const char*, int position);

    void setTotalsTable();
private slots:
	void openStatementImporter();
   // void statementItemClicked(QListWidgetItem *item);
    void openStatements();
    void changedStatement(std::string name);

};
#endif // MAINWINDOW_H
