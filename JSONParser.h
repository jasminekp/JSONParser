#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>

#include "rapidjson/document.h"
using namespace rapidjson;



class JSONParser
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
	Document document;


public:

	//initialize constructor
	JSONParser(const char* jsonData);


	//functions to modify/retrieve from the JSON data
	bool checkJSONValid(const char* jsonData);
	void storeJSON();
	std::string queryJSON(const std::string& queryDate);
	void addNewJSON(std::string& gameDate, std::string& visTeamName, std::string& homeTeamName, std::string& isNeutral, std::string& isFinal);
	bool checkNewDateFormat(std::string& newGameDate);

	std::string getLatestJSON();
	std::string getIndividualStat(MatchUpStats& game);
	std::string getAllStats();

	std::string determineStr(bool val);
	bool determineVal(std::string str);


	void parseTeamStats(int i, const char* arrName, MatchUpStats &MatchStats);

};

