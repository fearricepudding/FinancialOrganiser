#ifndef STATEMENT_H_INCLUDED
#define STATEMENT_H_INCLUDED

#include <json/json.h>
#include <string> 

#include "helpers.h"

class Statement{
public: 
    Statement();
    Statement(std::string data);
    Statement(Json::Value data);

    Json::Value getTransactions();
    int getTotal();
    void save();
    Json::Value addReccuringTransactions();

private:
    Json::Value data;

};

#endif