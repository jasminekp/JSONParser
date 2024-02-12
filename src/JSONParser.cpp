/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #1
 * 2/4/2024
 *
 * This JSONParser.cpp file contains all the implementation for the JSONParser class. The JSONParser class handles the parsing, storing,
 * retrieving, and generating of the JSON Data. A rapidjson document object is used to initially parse the JSON string, and all data is then
 * stored individually to a vector of MatchUpStats structs. Each struct contains the lower level objects from the JSON data.
 * 
 **/

#include "JSONParser.h"

/*
 * The JSONParser constructor takes the unparsed string of JSON data and initializes the success variable, and the vector of MatchUpStats structs
 * the constructor then checks if the JSON data can be parsed and stores the data into the struct 
 */
JSONParser::JSONParser(const char* jsonData) 
{
	this->success = false;
	allMatchStats = std::vector<MatchUpStats>();
	
	if (checkJSONValid(jsonData))
	{
		storeJSON();
	}
}

/*
 * This function determines the string equivalent of the boolean variable passed in (0 = false, 1 = true) 
 */
std::string JSONParser::determineStr(bool val)
{
	return (val == false) ? "false" : "true";
}

/*
 * This function determines the boolean equivalent of the string variable passed in (false = 0, true = 1)
 */
bool JSONParser::determineVal(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::toupper(c);
	});
		
	return (str == "YES") ? true : false;
}

/*
 * The checkJSONValid function determines if the unparsed string of JSON data can actually be parsed by the rapidjson document
 * The rapidjson document is used to parse the string. An error is thrown if the data is not valid
 */
bool JSONParser::checkJSONValid(const char* jsonData)
{
	document.Parse(jsonData);

	if (!document.IsObject() || document.HasParseError())
	{
		throw "\nERROR: Failed to parse JSON: " + (document.GetParseError());
	}
	else 
	{
		return true;
	}

}

/*
 * The storeJSON function iterates through the higher level JSON data array and stores each parameter into the associating
 * struct parameter. For example, the rapidjson's date object will be stored in the MatchUpStats struct's date object.
 * The individual homeTeamStats and visStats data is parsed and stored through the parseTeamStats function
 */
void JSONParser::storeJSON()
{
	const Value& matchUpStatsArr = document["matchUpStats"];

	if (document.HasMember("success"))
	{
		this->success = document["success"].GetBool();
	}
	
	for (SizeType i = 0; i < matchUpStatsArr.Size(); i++)
	{
		MatchUpStats MatchStats;

		if (matchUpStatsArr[i].IsObject()) 
		{
			MatchStats.neutral = matchUpStatsArr[i]["neutral"].GetBool();
			MatchStats.visTeamName = matchUpStatsArr[i]["visTeamName"].GetString();
			parseTeamStats(i, "visStats", MatchStats);
			parseTeamStats(i, "homeStats", MatchStats);
			MatchStats.homeTeamName = matchUpStatsArr[i]["homeTeamName"].GetString();
			MatchStats.isFinal = matchUpStatsArr[i]["isFinal"].GetBool();
			MatchStats.date = matchUpStatsArr[i]["date"].GetString();
		}

		allMatchStats.push_back(MatchStats);
	}
}


/*
 * The parseTeamStats function takes the following:
 *		1) the name of the team (visStats or homeTeamStats)
 *		2) the current index of the struct within the vector of structs
 *		3) a reference to the current MatchUpStats struct being parsed
 * 
 * The parseTeamStats function iterates each item belonging to the visStats and homeTeamStats and stores the item name as well as
 * value to the unordered maps within the MatchUpStats struct
 */
void JSONParser::parseTeamStats(int i, const char* arrName, MatchUpStats& MatchStats)
{
	for (Value::ConstMemberIterator it = document["matchUpStats"][i][arrName].MemberBegin(); it != document["matchUpStats"][i][arrName].MemberEnd(); ++it)
	{
		//check if the value stored is an integer or string and store accordingly
		std::string convertedVal = (it->value.IsInt()) ? std::to_string(it->value.GetInt()) : (it->value.GetString());

		if (arrName == "visStats")
		{
			MatchStats.visStats.insert(std::make_pair<std::string, std::string>(it->name.GetString(), convertedVal.c_str()));
		}
		else 
		{
			MatchStats.homeStats.insert(std::make_pair<std::string, std::string>(it->name.GetString(), convertedVal.c_str()));
		}
	}
}

/*
 * The getAllStats function iterates through the vector of MatchUpStats struct named allMatchStats. As each vector is iterated,
 * the formatted data is added to the allStats string. At the end, the allStats string is returned by the function
 */
std::string JSONParser::getAllStats()
{
	std::string allStats = "";

	for (auto& game : allMatchStats)
	{
		allStats += getIndividualStat(game);
	}
	return allStats;
}

/*
 * The getIndividualStat function iterates the data belonging to the MatchUpStats struct passed in as an argument 
 * The formatted results are then returned by the function as a string
 */
std::string JSONParser::getIndividualStat(MatchUpStats& game)
{
	std::string jsonStr = "";
	jsonStr += "GAME STATS ON " + game.date + ":\n";
	jsonStr += "--------------------------\n";
	jsonStr += "Neutral: " + determineStr(game.neutral);

	jsonStr += "\nVisiting Team: " + game.visTeamName + "\n";
	//iterate the items within the visiting team unordered map
	for (auto visTeam = game.visStats.begin(); visTeam != game.visStats.end(); ++visTeam)
	{
		jsonStr += visTeam->first + ": " + visTeam->second + "\n";
	}

	jsonStr += "\nHome Team: " + game.homeTeamName + "\n";
	//iterate the items within the home team unordered map
	for (auto homeTeam = game.visStats.begin(); homeTeam != game.visStats.end(); ++homeTeam)
	{
		jsonStr += homeTeam->first + ": " + homeTeam->second + "\n";
	}

	jsonStr += "Final: " + determineStr(game.isFinal);
	jsonStr += "\n--------------------------\n\n";
	
	return jsonStr;
}

/*
 * The queryJSON function queries the JSON data that is stored within the allMatchStats vector of MatchUpStats struct
 * Once the matching struct is found, the struct's data is returned as a formatted string
 */
std::string JSONParser::queryJSON(const std::string& queryDate)
{
	auto matchedGame = std::find_if(allMatchStats.begin(), allMatchStats.end(), [queryDate](const MatchUpStats& game) 
						{ return game.date == queryDate; });
	
	//throw an error if the struct cannot be found with the associated query date
	if (matchedGame == allMatchStats.end()) 
	{
		throw "No matches were found with this date.";
	}

	MatchUpStats game = *matchedGame;
	return "\n" + getIndividualStat(game);
}

/*
 * The getLatestJSON function gets the last MatchUpStats struct within the allMatchStats vector
 * This last struct is then passed to the getIndividualStat function and the formatted contents are returned
 * as a string
 */
std::string JSONParser::getLatestJSON()
{
	auto& latest = allMatchStats.back();

	MatchUpStats latestAddition = latest;
	return "\n" + getIndividualStat(latestAddition);
}

/*
 * checkNewDateFormat function checks the date passed in as a string argument and determines if it is
 * in a YYYY-MM-DD format using regex
 */
bool JSONParser::checkNewDateFormat(std::string& newGameDate)
{
	std::regex pattern("\\b\\d{4}-\\d{2}-\\d{2}\\b");
	return std::regex_match(newGameDate, pattern);
}


/*
 * The addNewJSON function takes higher level JSON input information such as the game date, visiting team name, home team name, etc.
 * The statcode and gamecode are generated by replacing the date with four zeros. 
 * For reusability purposes, the lower level data for the visiting team & home team are copied from the first struct in the allMatchStats struct
 *
 * Once all data has been generated for this new game, a newMatch struct is created and added to the vector of MatchUpStats structs
 */
void JSONParser::addNewJSON(std::string& gameDate, std::string& visTeamName, std::string& homeTeamName, std::string& isNeutral, std::string& isFinal)
{
	if (!checkNewDateFormat(gameDate))
	{
		throw "\nERROR: Match date is not in YYYY-MM-DD format.";
	}

	std::string statCode, gameCode;
	std::unordered_map<std::string, std::string> newVisStats = allMatchStats[0].visStats;
	std::unordered_map<std::string, std::string> newHomeStats = allMatchStats[0].homeStats;

	statCode = gameDate;
	statCode = std::regex_replace(gameDate, std::regex("-"), "0000");

	gameCode = "00" + statCode;
	newVisStats.emplace("statIdCode", statCode).first->second = statCode;
	newVisStats.emplace("gameCode", gameCode).first->second = gameCode;

	MatchUpStats newMatch = { determineVal(isNeutral), visTeamName, newVisStats, homeTeamName, newHomeStats, determineVal(isFinal), gameDate };

	allMatchStats.push_back(newMatch);

}



