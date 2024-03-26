/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #4
 * 3/24/2024
 *
 * This JSONParser.h header file contains the private and public data members/functions for the JSONParser class.
 **/

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

	//struct holds the individual/lower level information from the JSON data
	struct MatchUpStats {
		bool neutral;
		std::string visTeamName;
		std::unordered_map<std::string, std::string> visStats;
		std::string homeTeamName;
		std::unordered_map<std::string, std::string> homeStats;
		bool isFinal;
		std::string date;
	};

	//allMatchStats vector of MatchUpStats stores ALL the match up statistics data from the JSON object
	std::vector<MatchUpStats> allMatchStats;
	bool success;
	Document document;


public:

	//initialize constructor
	JSONParser(const char* jsonData);
  JSONParser(){}


	//functions to modify/retrieve from the JSON data
	bool checkJSONValid(const char* jsonData);
	void storeJSON();
	std::string queryJSON(const std::string& queryDate);
	void addNewJSON(std::string& gameDate, std::string& visTeamName, std::string& homeTeamName, std::string& isNeutral, std::string& isFinal);
	bool checkNewDateFormat(std::string& newGameDate);
	void parseTeamStats(int i, const char* arrName, MatchUpStats& MatchStats);
  void parse(const char* jsonData);

	//getters
	std::string getLatestJSON();
	std::string getIndividualStat(MatchUpStats& game);
	std::string getAllStats();
  
  std::vector<MatchUpStats> getMatchUpStats();
	

	//functions that convert the boolean to string equivalent and vice-versa
	std::string determineStr(bool val);
	bool determineVal(std::string str);

};

