/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #2
 * 3/3/2024
 *
 * This Main.cpp file contains the main function of the JSONParser program as well as program specific prompts for the user. The purpose of this JSONParser application is to
 * retrieve a json file from a web URL and parse the file, and either: 1) store the parsed JSON objects to a file, 2) query the JSON objects based on a date input, or 3) add a new
 * lower level JSON object (specifically a 'matchUpStats' object) to the higher level JSON Object.
 **/

#include "JSONParser.h"
#include "FileHandler.h"
#include <curl/curl.h>
#include <queue>
#include <unordered_set>

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
 * The getTeamRecords function takes in the jsonParser object by reference and calculates the each team's wins, losses, and ties.
 * The function iterates each game within each matchUpStats object and stores the information for each team within the records struct
 * All calculations are then printed to the console in a table format
 */
void getTeamRecords(JSONParser& jsonParser)
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
    printf("\n%-10s %8s %8s %8s %8s\n", "TeamName", "TeamCode", "Win", "Loss", "Tie");
    printf("-------------------------------------------------\n");
    for (auto recordData : teamRecords)
    {
        auto record = recordData.second;
        printf("%-10s %8d %8d %8d %8d\n", record.teamName.c_str(), recordData.first, record.win, record.loss, record.tie);
    }

}



/*
 * This function determines if the user wants to print the JSON data to a file (1) or pull each team's score records (2).
 * The 32 websites, each containing data for each team, are iterated through a for loop and added to a message queue for processing, then
 * parsed through the JSONParser class.
 */
void promptAllOptions(int selectedOption)
{
    JSONParser jsonParser;
    FileHandler fileHandler;
    std::vector<std::string> responses = std::vector<std::string>();
    std::queue<std::string> messageQueue;

    std::cout << "Please wait while all data is being retrieved..." << std::endl;
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

    if (selectedOption == 1)
    {
        std::string outputPath;
        std::cout << "Enter the output file path: ";
        std::cin >> outputPath;

        if (fileHandler.directoryExist(outputPath))
        {
            fileHandler.write(jsonParser.getAllStats());
            std::cout << "\nSuccess! All JSON Data has been written to " + fileHandler.getFileName() << std::endl;
        }
    }
    else if (selectedOption == 2)
    {
        getTeamRecords(jsonParser);
    }
}


int main()
{
    int selectedOption = 0;

    //provide a welcome message to the user and prompt the user to enter a web URL
    std::cout << "Welcome to the JSON Parser!\n";
    std::cout << "This program will iterate through all 32 sports.snoozle.net websites and perform the following requests.\n";

    do
    {
        //ask user what they want to do: generate, retrieve, or read the JSON data, or or enter -1 to exit the program
        std::cout << "\n\nEnter 1, or 2 for the options below, OR enter -1 to quit this program: \n";
        std::cout << "1. Read all the JSON Data to a file.\n";
        std::cout << "2. Pull team records.\n\n";
        std::cin >> selectedOption;
        std::cout << std::endl;

        try
        {
            promptAllOptions(selectedOption);
        }
        catch (const char* message)
        {
            std::cerr << message << std::endl;
            selectedOption = 0;
        }

    } while (selectedOption != -1);

}



