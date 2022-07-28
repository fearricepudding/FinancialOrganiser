#include "bill.h"

Bill::Bill(std::string ref, float total){
    this->reference = ref;
    this->total = total;
};

Bill::Bill(std::string ref, float total, std::string label){
    Bill(ref, total);
    this->label = label;
};

Bill::Bill(std::string ref, float total, std::string label, int occurances){
    Bill(ref, total, label);
    this->occurances = occurances;
};

void Bill::setLabel(std::string label){
    this->label = label;
};

std::string Bill::getLabel(){
    return this->label;
};