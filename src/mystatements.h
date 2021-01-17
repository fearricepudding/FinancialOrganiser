#ifndef MYSTATEMENTS_H
#define MYSTATEMENTS_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class myStatements;
}

class myStatements : public QDialog
{
    Q_OBJECT

public:
    explicit myStatements(QWidget *parent = nullptr);
    ~myStatements();
    std::string currentStatement = "";

private:
    Ui::myStatements *ui;

private slots:
    void selectedSlot();
    void statementClicked(QListWidgetItem *item);

signals:
    void newStatement(std::string statement);
};

#endif // MYSTATEMENTS_H
