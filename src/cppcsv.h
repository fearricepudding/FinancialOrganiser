#include <iostream>
#include <stdio.h>
#include <json/json.h>
#include <fstream>

class csvReader{
public:
	csvReader(){
		std::cout << "[*] CSVREADER" << std::endl;
	}
	
	/**
	 * load CSV file
	 * @param String path
	 * @return Json::Value
	 */
	Json::Value readFile(const std::string filepath, const std::string delim){
		std::cout << "[*] Reading file: " << filepath << std::endl;
		std::ifstream infile(filepath);
		std::string line; 
		Json::Value fileData;
		std::vector<std::string> headdings;
		int valueLine = 0; 
		int lineNum = 1;
		while(std::getline(infile, line)){
			std::cout << lineNum << std::endl;
			if(lineNum == 1){
				// Get the headdings from the first line
				headdings = split(line, delim);
			}else{
				Json::Value currentObj;
				std::vector<std::string> currentValues = split(line, delim);
				for(int i = 0; i < headdings.size(); i++){
					currentObj[headdings[i]] = currentValues[i];
				};
				fileData[valueLine] = currentObj;
				valueLine++;
			};
			lineNum++;
		};
		return fileData;
	};

	/**
	 * Split line by delim
	 * @param String s string to split
	 * @param Char delim to split by
	 * @return std::vector of strings
	 */
	std::vector<std::string> split(const std::string s, std::string delim){
		std::vector<std::string> results;
		int currentPos = 0; 
		int newPos;
		bool found = true;
		while(found){
			found = false; 
			newPos = s.find(delim, currentPos);
			int length = newPos - currentPos;
			std::string token = s.substr(currentPos, length);
			results.push_back(token);
			if(newPos <= 0){
				break;
			}else{
				found = true;
			};
			currentPos = newPos+1;
		};
		return results;
	};

	/**
	 * Stringify json
	 * @param Json::Value to stringify
	 * @return std::string
	 */
	std::string stringify(Json::Value in){
		Json::StreamWriterBuilder builder;
		builder.settings_["indentation"] = "";
		std::string out = Json::writeString(builder, in);
		return out; 
	}
}; // end of csvReader

