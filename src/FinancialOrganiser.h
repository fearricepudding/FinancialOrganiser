#ifndef FINANCIALORGANISER_H
#define FINANCIALORGANISER_H
#include <QWidget>

class FinancialOrganiser: public QWidget{
public:
	FinancialOrganiser();
	~FinancialOrganiser();
	static bool fileExists(std::string filepath);

public slots:
    int importStatement();

};

#endif
