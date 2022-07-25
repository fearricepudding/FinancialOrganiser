#include "database.h"

database* database::m_pInstance = NULL;

database* database::instance(){
	if(!m_pInstance)
		m_pInstance = new database();
	return m_pInstance;
};

bool database::fileExists(const std::string& name){
    std::ifstream f(name.c_str());
    return f.good();
}

database::database(){
	 if(fileExists("FO.bin")){
         readSaveFile();
     };
     Json::Value deb = new Json::Value();
};

char encrypt( char c ) { return c + 1; }        
char decrypt( char c ) { return c - 1; }   

database::~database(){};

void database::save(){
    writeToSave();
};

void database::writeToSave(){
	std::string out = Helpers::stringify(this->state);
    std::ofstream file("FO.bin", std::ios_base::binary);
    std::ostream_iterator<char> test(file);
    std::string enc;
    enc.resize(out.size());
    transform(out.begin(), out.end(), enc.begin(), encrypt);
    file.write(enc.c_str(), enc.size());
};

void database::readSaveFile(){
    std::ifstream file("FO.bin", std::ios::binary);
    if(file.good()){
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        std::string enc(buffer.begin(), buffer.end());
        std::string data;
        data.resize(enc.size());
        transform(enc.begin(), enc.end(), data.begin(), decrypt);
        this->state = Helpers::parseJson(data);
    }else{
        dbg->err("Failed to read database");
    };
}

void database::loadSavedState(){
	readSaveFile();
};

void database::addStatement(Json::Value data, std::string name){
    this->state["statements"][name] = data;
};

std::string database::getStateAsString(){
	return Helpers::stringify(this->state);
};

Statement database::getStatement(std::string statementName){
    dbg->out("Getting statement data");
    Json::Value statementData = this->state["statements"][statementName];
    return Statement(statementData);
};

Json::Value database::getState(){
    return  this->state;
};

Json::Value database::getAllStatements(){
    return this->state["statements"];
};

Json::Value database::getStatementNames(){
    Json::Value statements = this->state["statements"];
    Json::Value names;
    for(int i = 0; i < statements.size(); i++){
        std::string name = statements.getMemberNames()[i];
        names[i] = name;
    }
    return names;
};

Json::Value database::getBills() {
    Json::Value bills = this->state["bills"];
    return bills;
}

void database::addBills(Json::Value bills) {
    Json::Value current = this->getBills();
    std::cout << current << std::endl;
    for(std::string bill : bills.getMemberNames()){
        std::string value = bills[bill].asString();
        current[bill] = value;
    }
    this->state["bills"] = current;
    this->save();
}