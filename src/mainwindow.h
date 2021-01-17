#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "importstatement.h"
#include "database.h"
#include "mystatements.h"
#include <QListWidgetItem>

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
    std::string selectedStateament = "tmp";
    void clearStats();

private slots:
	void openStatementImporter();
    void statementItemClicked(QListWidgetItem *item);
    void openStatements();
    void changedStatement(std::string name);

};
#endif // MAINWINDOW_H
