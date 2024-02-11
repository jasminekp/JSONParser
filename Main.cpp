/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University, Professor Greg Wagner
 * Project #1
 * 2/4/2024
 *
 * OOP Project that parses JSON input. 
 * 
 **/

#include "JSONParser.h"
#include <curl/curl.h>
#include "FileHandler.h"


size_t callBackCurl(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

void connectToWebAPI(std::string webURL, std::string &response)
{
    //temp
    webURL = "https://sports.snoozle.net/search/nfl/searchHandler?fileType=inline&statType=teamStats&season=2020&teamName=26";

    //initiate the curl command
    CURL* curl = curl_easy_init();

    const char* url = webURL.c_str();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackCurl);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);

    //cleanup cURL
    curl_easy_cleanup(curl);
}

void promptNewGame(JSONParser& json)
{
    std::string gameDate, visTeamName, homeTeamName, isNeutral, isFinal;

    std::cout << "Please enter the following (note: stat ID and game codes are auto generated): \n";
    std::cout << "Game Date: ";
    std::cin >> gameDate;
    std::cout << "\nNeutral? Yes or No ";
    std::cin >> isNeutral;
    std::cout << "\nVisiting Team Name: ";
    std::cin >> visTeamName;
    std::cout << "\nHome Team Name: ";
    std::cin >> homeTeamName;
    std::cout << "\nResults Final?: ";
    std::cin >> isFinal;

    std::cout << "\nThank you. The results for each team have been generated based on the game date.\n";
    json.addNewJSON(gameDate, visTeamName, homeTeamName, isNeutral, isFinal);

   // std::cout << json.ToString();
}

int main()
{
    std::string webLink;
    std::string response;
    int selectedOption;

    //provide a welcome message to the user and enter the link to the URL
    std::cout << "Welcome to the JSON Parser!" << std::endl;
    std::cout << "Please enter the web link to the JSON data: ";
    getline(std::cin, webLink);

    connectToWebAPI(webLink, response);

    //add the json data to the object
    JSONParser json(response.c_str());


    //ask user what they want to do: generate, retrieve, read, etc.
    std::cout << "Enter 1, 2, or 3 for the options below:\n";
    std::cout << "1. Read all the JSON Data to a file.\n";
    std::cout << "2. Query the JSON object and retrieve its associated data.\n";
    std::cout << "3. Add new JSON data based on a game previously played.\n";
    std::cin >> selectedOption;

    if (selectedOption == 1)
    {
        std::string outputPath;

        std::cout << "Enter the output file path: ";
        std::cin >> outputPath;

        FileHandler fileWriter(outputPath);

        if (fileWriter.directoryExist())
        {
            fileWriter.write(json.getAllStats());
        }
    }
    else if (selectedOption == 2)
    {
        std::string gameDate;

        std::cout << "Please enter the game date as YYYY-MM-DD format: ";
        std::cin >> gameDate;
      
        std::string queriedData = json.queryJSON(gameDate);
        std::cout << queriedData << std::endl;
        
    }
    else if (selectedOption == 3)
    {
        promptNewGame(json);
        
    }
    
}



