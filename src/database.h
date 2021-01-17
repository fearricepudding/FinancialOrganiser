#ifndef DATABASE_H
#define DATABASE_H

#include <json/json.h>

class database{
public:
	static database* instance();
	int loadSavedState();
	int save();
	Json::Value getState();
	int addStatement(Json::Value data, std::string name);
	std::string getStateAsString();
    Json::Value getStatement(std::string statementName);

private:
	static database* m_pInstance;
	database();
	~database();
	FILE* savefile;
	int openSave();
	int closeSave();
	int writeToSave();
	Json::Value state;
	int createNewSave();
	bool fileExists(std::string filepath);
	int readSaveFile();
	std::string stringify(Json::Value in);

};

#endif
