#include "database.h"
#include <stdio.h>
#include <iostream>

database* database::m_pInstance = NULL;

database* database::instance(){
	if(!m_pInstance)
		m_pInstance = new database();
	return m_pInstance;
};

bool database::fileExists(std::string filepath){
	if (FILE *file = fopen(filepath.c_str(), "r")) {
        fclose(file);
        return true;
    }else{
        return false;
    };
};

database::database(){
	 if(!fileExists("./FinancialOrganiser.dat")){
	 	createNewSave();
     }else{
         loadSavedState();
     };
};

database::~database(){};

int database::save(){
	openSave();
	writeToSave();
	closeSave();
	return 0;
};

int database::openSave(){

	if(savefile!=NULL){
		return 0;
	}else{
		return 1; // File no exists?
	};
};

int database::closeSave(){
	fclose(savefile);
    savefile = NULL;
	return 0;
};

int database::writeToSave(){
    savefile = fopen("./FinancialOrganiser.dat", "w");
	std::string out = stringify(state);
	const void * buffer = out.c_str();
	fwrite(buffer, 1, out.length(), savefile);
	return 0;
};

std::string database::stringify(Json::Value in){
	Json::StreamWriterBuilder builder;
	builder.settings_["indentation"] = "";
	std::string out = Json::writeString(builder, in);
	return out;
};

int database::createNewSave(){
	savefile = fopen("FinancialOrganiser.dat", "ab+");
    closeSave();
	return 0;
};

int database::readSaveFile(){
    savefile = fopen("./FinancialOrganiser.dat", "ab+");
	std::stringstream buffer;
    char str[10000];
    while (fread(str, 1, 1000, savefile) != NULL){
        buffer << str;
    };
	std::string value = buffer.str();
	Json::Reader reader;
    bool b = reader.parse(value, state);
    return b;
};

int database::loadSavedState(){
	readSaveFile();
	closeSave();
	return 0;	
};

int database::addStatement(Json::Value data, std::string name){
	state[name] = data;
	return 0;
};

std::string database::getStateAsString(){
	return stringify(state);
};

Json::Value database::getStatement(std::string statementName){
    Json::Value statement = state[statementName];
    return statement;
};

Json::Value database::getState(){
    return state;
};
