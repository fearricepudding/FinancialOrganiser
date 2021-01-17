#ifndef CSVPARSER_H_INCLUDED
#define CSVPARSER_H_INCLUDED

#include "database.h"

class CsvParser{
public: 
	CsvParser();
	CsvParser(std::string filename);
	~CsvParser();
	int setFilePath(std::string filepath);
	std::string getFilePath();
	std::string err;
	Json::Value readFile();
	int loadStatement(std::string name);
private:
	std::string path;
	bool fileExists(std::string filepath);
	std::vector<std::string> split(std::string s, std::string delim);
	database *db = database::instance();
	std::string stringify(Json::Value in);
};

#endif