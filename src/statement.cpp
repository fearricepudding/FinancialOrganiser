#include "statement.h"

Statement::Statement(){};

Statement::Statement(std::string dta){
    this->data = Helpers::parseJson(dta);
};

Statement::Statement(Json::Value dta){
    this->data = dta;
};

Json::Value Statement::getTransactions(){
    return this->data;
}