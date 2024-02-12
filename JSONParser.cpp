#include "JSONParser.h"


JSONParser::JSONParser(const char* jsonData) 
{
	this->success = false;
	allMatchStats = std::vector<MatchUpStats>();
	
	if (checkJSONValid(jsonData))
	{
		storeJSON();
	}
}

std::string JSONParser::determineStr(bool val)
{
	return (val == false) ? "false" : "true";
}

bool JSONParser::determineVal(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::toupper(c);
	});
		
	return (str == "YES") ? true : false;
}


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

//this function stores the items in the struct, for file info
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


void JSONParser::parseTeamStats(int i, const char* arrName, MatchUpStats& MatchStats)
{
	for (Value::ConstMemberIterator it = document["matchUpStats"][i][arrName].MemberBegin(); it != document["matchUpStats"][i][arrName].MemberEnd(); ++it)
	{
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

std::string JSONParser::getAllStats()
{
	std::string allStats = "";

	for (auto& game : allMatchStats)
	{
		allStats += getIndividualStat(game);
	}
	return allStats;
}


std::string JSONParser::getIndividualStat(MatchUpStats& game)
{
	std::string jsonStr = "";
	jsonStr += "GAME STATS ON " + game.date + ":\n";
	jsonStr += "--------------------------\n";
	jsonStr += "Neutral: " + determineStr(game.neutral);

	jsonStr += "\nVisiting Team: " + game.visTeamName + "\n";
	for (auto visTeam = game.visStats.begin(); visTeam != game.visStats.end(); ++visTeam)
	{
		jsonStr += visTeam->first + ": " + visTeam->second + "\n";
	}

	jsonStr += "\nHome Team: " + game.homeTeamName + "\n";
	for (auto homeTeam = game.visStats.begin(); homeTeam != game.visStats.end(); ++homeTeam)
	{
		jsonStr += homeTeam->first + ": " + homeTeam->second + "\n";
	}

	jsonStr += "Final: " + determineStr(game.isFinal);
	jsonStr += "\n--------------------------\n\n";
	
	return jsonStr;
}


std::string JSONParser::queryJSON(const std::string& queryDate)
{
	auto matchedGame = std::find_if(allMatchStats.begin(), allMatchStats.end(), [queryDate](const MatchUpStats& game) 
						{ return game.date == queryDate; });
	
	if (matchedGame == allMatchStats.end()) 
	{
		throw "No matches were found with this date.";
	}

	MatchUpStats game = *matchedGame;
	return "\n" + getIndividualStat(game);
}

std::string JSONParser::getLatestJSON()
{
	auto& latest = allMatchStats.back();

	MatchUpStats latestAddition = latest;
	return "\n" + getIndividualStat(latestAddition);
}


bool JSONParser::checkNewDateFormat(std::string& newGameDate)
{
	std::regex pattern("\\b\\d{4}-\\d{2}-\\d{2}\\b");
	return std::regex_match(newGameDate, pattern);
}


//adds a new json object
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



