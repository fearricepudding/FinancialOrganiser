#include "database.h"
#include "FinancialOrganiser.h"
#include <stdio.h>
#include <iostream>

database* database::m_pInstance = NULL;

database* database::instance(){
	if(!m_pInstance)
		m_pInstance = new database();
	return m_pInstance;
}

bool database::fileExists(std::string filepath){
	if (FILE *file = fopen(filepath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
    	//err = "Failed to open file: File doesn't exist.";
        return false;
    } 
}

database::database(){
	 if(!fileExists("./FinancialOrganiser.dat")){
	 	createNewSave();
     }else{
         loadSavedState();
     }
};
database::~database(){};

int database::save(){
	openSave();
	writeToSave();
	closeSave();
	return 0;
}

int database::openSave(){
	savefile = fopen("./FinancialOrganiser.dat", "ab+");
	if(savefile!=NULL){
		return 0;
	}else{
		return 1; // File no exists?
	};
};

int database::closeSave(){
	fclose(savefile);
	return 0;
};

int database::writeToSave(){
	std::string out = stringify(state);
	const void * buffer = out.c_str();
	// Write to file
	fwrite(buffer, 1, out.length(), savefile);
	return 0;
};

std::string database::stringify(Json::Value in){
	Json::StreamWriterBuilder builder;
	builder.settings_["indentation"] = "";
	// Stringify the json value
	std::string out = Json::writeString(builder, in);
	//std::cout << out << std::endl;
	out.erase(std::find(out.begin(), out.end(), '\0'), out.end());
	out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
	return out;
}

int database::createNewSave(){
	savefile = fopen("FinancialOrganiser.dat", "ab+");
	fclose(savefile);
	return 0;
}

int database::readSaveFile(){
	std::stringstream buffer;
    char str[10000];

    while (fread(str, 1, 1000, savefile) != NULL){
        buffer << str;
    }

	std::string value = buffer.str();
	//std::cout << "Read: " << value << std::endl;
	Json::Reader reader;
    bool b = reader.parse(value, state);

	return 0;

}

int database::loadSavedState(){
	openSave();
	readSaveFile();
	closeSave();
	return 0;	
};

int database::addStatement(Json::Value data, std::string name){
	state[name] = data;
	return 0;
}

std::string database::getStateAsString(){
	return stringify(state);
}

Json::Value database::getStatement(std::string statementName){
    Json::Value statement = state[statementName];
   // std::cout << stringify(statement) << std::endl;
    return statement;
}

Json::Value database::getState(){
    return state;
}
