#include "bill.h"

Bill::Bill(){};

Bill::Bill(std::string ref, std::string total){
    this->data["reference"] = ref;
    this->data["total"] = total;
};

Bill::Bill(std::string ref, std::string total, std::string label){
    Bill(ref, total);
    this->data["label"] = label;
};

Bill::Bill(std::string ref, std::string total, std::string label, int occurances){
    Bill(ref, total, label);
    this->data["occurances"] = occurances;
};

Bill::Bill(Json::Value data){
    this->data = data;
};

void Bill::setLabel(std::string label){
    this->data["label"] = label;
};

std::string Bill::getLabel(){
    if(this->data.isMember("label")){
        return this->data["label"].asString();
    };
    return NULL;
};

void Bill::setReference(std::string ref){
    this->data["reference"] = ref;
};

std::string Bill::getReference(){
    return this->data["reference"].asString();
};

void Bill::setTotal(int total){
    this->data["total"] = total;
}

std::string Bill::getTotal(){
    return this->data["total"].asString();
}

bool Bill::checkForReference(std::vector<Bill> *bills, std::string needle){
    for(int i = 0; i < bills->size(); i++){
        Bill bill = bills->at(i);
        std::string ref = bill.getReference();
        if(ref == needle){
            return true;
        }
    }
    return false;
};

Json::Value Bill::getState(){
    return this->data;
}