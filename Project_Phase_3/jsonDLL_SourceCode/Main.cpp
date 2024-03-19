/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #3
 * 3/17/2024
 *
 * This Main.cpp file contains the main function of the JSONParser program as well as program specific prompts for the user. The purpose of this JSONParser application is to
 * retrieve a json file from a web URL and parse the file, and print the team match records (wins, losses, ties, etc).
 **/

#include "JSONParser.h"
#include "FileHandler.h"
#include <curl/curl.h>
#include <queue>
#include <unordered_set> 

std::string teamData;

/*
 * callBackCurl function is called by libcurl in order to save the json data from the web URL provided by the user
 */
 size_t callBackCurl(void* contents, size_t size, size_t nmemb, std::string* output) {
     size_t total_size = size * nmemb;
     output->append((char*)contents, total_size);
     return total_size;
 }

/*
 * The connectToWebAPI function takes in the web URL that holds the JSON data and returns the response from the web as a string
 * A CURL command is used to make the connection to the web URL API
 */
std::string connectToWebAPI(std::string url)
{
    std::string response;

     //initiate the curl command
     CURL* curl = curl_easy_init();
     curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackCurl);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
     CURLcode res = curl_easy_perform(curl);

     if (!res == CURLE_OK)
     {
         throw "\nERROR: website is not valid. Exiting Program.";
     }

     //cleanup cURL
     curl_easy_cleanup(curl);

    return response;

}


/*
 * The getTeamRecords function takes in the jsonParser object by reference and calculates the each team's wins, losses, and ties.
 * The function iterates each game within each matchUpStats object and stores the information for each team within the records struct
 * All calculations are then printed to the console in a table format
 */
std::string getTeamRecords(JSONParser& jsonParser)
{
    auto data = jsonParser.getMatchUpStats();
    struct records
    {
        int win;
        int loss;
        int tie;
        std::string teamName;
    };

    std::unordered_set<std::string> games;
    std::unordered_map<int, records> teamRecords;
    std::string formattedData;

    for (auto game : data)
    {
        if (games.find(game.visStats["gameCode"]) == games.end()) // if the game has not already been processed
        {
            int visitorScore = std::stoi(game.visStats["score"]);
            int visTeamCode = std::stoi(game.visStats["teamCode"]);
            int homeScore = std::stoi(game.homeStats["score"]);
            int homeTeamCode = std::stoi(game.homeStats["teamCode"]);

            //if the team has not already been stored, create a new records struct for storing data
            records visitorRecord = teamRecords.find(visTeamCode) != teamRecords.end() ? teamRecords[visTeamCode] : records();
            records homeRecord = teamRecords.find(homeTeamCode) != teamRecords.end() ? teamRecords[homeTeamCode] : records();

            visitorRecord.teamName = game.visTeamName;
            homeRecord.teamName = game.homeTeamName;

            if (homeScore > visitorScore)
            {
                homeRecord.win += 1;
                visitorRecord.loss += 1;
            }
            else if (homeScore < visitorScore)
            {
                homeRecord.loss += 1;
                visitorRecord.win += 1;
            }
            else
            {
                homeRecord.tie += 1;
                visitorRecord.tie += 1;
            }

            teamRecords[visTeamCode] = visitorRecord;
            teamRecords[homeTeamCode] = homeRecord;
        }
        games.insert(game.visStats["gameCode"]);
    }

    // print the name, code, wins, losses, and ties for each Team
    for (auto recordData : teamRecords)
    {
        auto record = recordData.second;
        formattedData += record.teamName + ",";
        formattedData += std::to_string(recordData.first) + "," + std::to_string(record.win) + "," + std::to_string(record.loss) + "," + std::to_string(record.tie) + ",";
    }

    return formattedData;
}

/*
 * The ProcessMessages function uses the message queue to regulate the curl requests going to the web API. 
 * The function only processes the data from the websites if the message queue is not empty.
 */
void ProcessMessages(std::queue<std::string>& messageQueue, std::vector<std::string>& responses)
{
    if (!messageQueue.empty()) {
        std::string message = messageQueue.front();
        messageQueue.pop();
        responses.emplace_back(connectToWebAPI(message));
    }

}

/*
 * This function pulls each team's score records.
 * The 32 websites, each containing data for each team, are iterated through a for loop and added to a message queue for processing, then
 * parsed through the JSONParser class.
 */
void getTeamInformation()
{
    JSONParser jsonParser;
    FileHandler fileHandler;
    std::queue<std::string> messageQueue;
    std::vector<std::string> responses = std::vector<std::string>();

    for (int i = 1; i <= 32; i++)
    {
        std::string webLink = "https://sports.snoozle.net/search/nfl/searchHandler?fileType=inline&statType=teamStats&season=2020&teamName=" + std::to_string(i);

        messageQueue.push(webLink);
        ProcessMessages(messageQueue, responses);
    }

    for (int i = 0; i <= 31; i++)
    {
        jsonParser.parse(responses[i].c_str());
        
    }

   teamData = getTeamRecords(jsonParser);

}

extern "C" { 
   const char* getTeamInfo() {
        getTeamInformation();
        return teamData.c_str(); 
    }
}



