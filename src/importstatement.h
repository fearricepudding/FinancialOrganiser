#ifndef IMPORTSTATEMENT_H
#define IMPORTSTATEMENT_H

#include <QDialog>

namespace Ui {
class importStatement;
}

class importStatement : public QDialog
{
    Q_OBJECT

public:
    explicit importStatement(QWidget *parent = nullptr);
    ~importStatement();

private:
    Ui::importStatement *ui;

private slots:
    void findStatement();
    void submitted();

signals:
    void newStatement(std::string statement);
};

#endif // IMPORTSTATEMENT_H
