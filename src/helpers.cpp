#include "helpers.h"

std::string Helpers::stringify(Json::Value in){
	Json::StreamWriterBuilder builder;
	builder.settings_["indentation"] = "";
	std::string out = Json::writeString(builder, in);
	return out;
};

Json::Value Helpers::parseJson(std::string string){
    Json::Value result;
    Json::Reader stringReader;
    stringReader.parse(string, result);
    return result;
};