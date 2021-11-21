#include "database.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <string> 
#include <sstream>

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
	std::string out = stringify(state);
    std::ofstream file("FO.bin", std::ios_base::binary);
    std::ostream_iterator<char> test(file);
    std::string enc;
    enc.resize(out.size());
    transform(out.begin(), out.end(), enc.begin(), encrypt);
    file.write(enc.c_str(), enc.size());
};

std::string database::stringify(Json::Value in){
	Json::StreamWriterBuilder builder;
	builder.settings_["indentation"] = "";
	std::string out = Json::writeString(builder, in);
	return out;
};

void database::readSaveFile(){
    std::ifstream file("FO.bin", std::ios::binary);
    if(file.good()){
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(file), {});
        std::string enc(buffer.begin(), buffer.end());
        std::string data;
        data.resize(enc.size());
        transform(enc.begin(), enc.end(), data.begin(), decrypt);
        Json::Reader stringReader;
        stringReader.parse(data, this->state);
    }else{
        dbg->err("Failed to read database");
    };
}

void database::loadSavedState(){
	readSaveFile();
};

void database::addStatement(Json::Value data, std::string name){
	state[name] = data;
};

std::string database::getStateAsString(){
	return stringify(state);
};

Json::Value database::getStatement(std::string statementName){
    dbg->out("Getting statement data");
    Json::Value statement = state[statementName];
    return statement;
};

Json::Value database::getState(){
    return state;
};
