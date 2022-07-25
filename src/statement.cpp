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

bool Statement::hasTransaction(std::string id){
    bool found = false;
    for (int i = 0; i < this->data.size(); i++){
        Json::Value item = this->data[i];
        if (!item["Transaction Description"].empty()){
            if(item["Transaction Description"].asCString() == id){
                found = true;
            }
        }
        if(found){
            return true;
        };
    };
    return false;
};