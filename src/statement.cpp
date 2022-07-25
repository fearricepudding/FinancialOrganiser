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

Json::Value Statement::addReccuringTransactions(){
    Json::Value reccuringTransactions = new Json::Value();
    // for(int i = 0; i < this->data.size(); i++){
    //     const char* transactionID = transaction["Transaction Description"].asCString();
    // }
    return reccuringTransactions;
}