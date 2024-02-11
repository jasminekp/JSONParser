#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <map>

#include "rapidjson/document.h"

class JsonProcessor

{
private:
	struct MatchUpStats {
		bool neutral;
		std::string visTeamName;
		std::unordered_map<std::string, std::string> visStats;
		std::string homeTeamName;
		std::unordered_map<std::string, std::string> homeStats;
		bool isFinal;
		std::string date;
	};

    //stores all the match up statistics data 
	std::vector<MatchUpStats> allMatchStats;
	bool success;

Public:

//initialize constructor
	JsonProcessor(const char* jsonData);

};