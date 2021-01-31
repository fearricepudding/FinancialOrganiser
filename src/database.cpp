#include "database.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
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

     Json::Value deb = getStatement("2");
     std::cout << stringify(deb) << std::endl;

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

#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

int database::readSaveFile(){
    savefile = fopen("FinancialOrganiser.dat", "r");
    char * saveFileData;
    size_t size;
    readall(savefile, &saveFileData, &size);

    Json::Reader reader;
    bool s = reader.parse(saveFileData, state);
    if(s != 0){
        std::cout << "Failed to parse savefile" << std::endl;
    }
    return s;
}


#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_ERROR      -2  /* Stream error */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

int database::readall(FILE *in, char **dataptr, size_t *sizeptr){
    char  *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (in == NULL || dataptr == NULL || sizeptr == NULL)
        return READALL_INVALID;
    /* A read error already occurred? */
    if (ferror(in))
        return READALL_ERROR;
    while (1) {
        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            /* Overflow check. Some ANSI C compilers
               may optimize this away, though. */
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }
            temp = (char*)realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }
        n = fread(data + used, 1, READALL_CHUNK, in);
        if (n == 0)
            break;
        used += n;
    }
    if (ferror(in)) {
        free(data);
        return READALL_ERROR;
    }
    temp = (char*)realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';
    *dataptr = data;
    *sizeptr = used;
    return READALL_OK;
}

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
