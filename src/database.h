#ifndef DATABASE_H
#define DATABASE_H

#include <jsoncpp/json/json.h>

class database{
public:
	static database* instance();
	void loadSavedState();
	void save();
	Json::Value getState();
	void addStatement(Json::Value data, std::string name);
	std::string getStateAsString();
    Json::Value getStatement(std::string statementName);

private:
	static database* m_pInstance;
	database();
	~database();
	void writeToSave();
	Json::Value state;
	bool fileExists(const std::string&);
	void readSaveFile();
	std::string stringify(Json::Value in);
    int readall(FILE *in, char **dataptr, size_t *sizeptr);
};

#endif
