#ifndef BILL_H
#define BILL_H

#include <iostream>
#include <json/json.h>
#include <string>

class Bill{
public:
    Bill();
    Bill(std::string, float);
    Bill(std::string, float, std::string);
    Bill(std::string, float, std::string, int);
    void setLabel(std::string);
    std::string getLabel();
private:
    float total;
    std::string label;
    std::string reference;
    int occurances;
};

#endif