#ifndef BILL_H
#define BILL_H

#include <iostream>
#include <json/json.h>
#include <string>

class Bill{
public:
    Bill();
    Bill(std::string, std::string);
    Bill(std::string, std::string, std::string);
    Bill(std::string, std::string, std::string, int);
    Bill(Json::Value);
    void setLabel(std::string);
    std::string getLabel();
    void setReference(std::string);
    std::string getReference();
    void setTotal(int);
    std::string getTotal();
    static bool checkForReference(std::vector<Bill>*, std::string);
    Json::Value getState();
private:
    Json::Value data;
};

#endif