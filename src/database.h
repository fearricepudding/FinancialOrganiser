#ifndef DATABASE_H
#define DATABASE_H

#include <json/json.h>
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

#include "debug.h"
#include "helpers.h"
#include "statement.h"

class database{
public:
	static database* instance();
	void loadSavedState();
	void save();
	Json::Value getState();
	void addStatement(Json::Value data, std::string name);
	std::string getStateAsString();
    Statement getStatement(std::string statementName);
	void addBills(Json::Value);
	Json::Value getBills();
	Json::Value getAllStatements();
	Json::Value getStatementNames();

private:
    debug *dbg = new debug(true, 2);
	static database* m_pInstance;
	database();
	~database();
	void writeToSave();
	Json::Value state;
	bool fileExists(const std::string&);
	void readSaveFile();
    int readall(FILE *in, char **dataptr, size_t *sizeptr);
};

#endif
