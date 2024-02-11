#include "JsonProcessor.h"

JsonProcessor::JsonProcessor(const char* jsonData) 
{

}
//this function stores the items in the struct, for file info
void JsonProcessor::storeJSON()
{
	const Value& matchUpStatsArr = document["matchUpStats"];
	this->success = document["success"].GetBool();
	
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
