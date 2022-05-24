#ifndef HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <json/json.h>
#include <string>

class Helpers{
public:
    static std::string stringify(Json::Value);
    static Json::Value parseJson(std::string);
};

#endif