/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #1
 * 2/4/2024
 *
 * This Main.cpp file contains the main function of the JSONParser program as well as program specific prompts for the user. The purpose of this JSONParser application is to
 * retrieve a json file from a web URL and parse the file, and either: 1) store the parsed JSON objects to a file, 2) query the JSON objects based on a date input, or 3) add a new 
 * lower level JSON object (specifically a 'matchUpStats' object) to the higher level JSON Object. 
 **/

#include "JSONParser.h"
#include "FileHandler.h"
#include <curl/curl.h>

/*
 * callBackCurl function is called by libcurl in order to save the json data from the web URL provided by the user
 */
size_t callBackCurl(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

/*
 * connectToWebAPI function takes in the web URL that holds the JSON data, as well as an empty response string that stores
 * the JSON data. a CURL command is used to make the connection to the web URL API
 */
void connectToWebAPI(std::string webURL, std::string &response)
{
    //initiate the curl command
    CURL* curl = curl_easy_init();

    const char* url = webURL.c_str();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callBackCurl);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    CURLcode res = curl_easy_perform(curl);

    if (!res == CURLE_OK)
    {
        throw "\nERROR: website is not valid. Exiting Program.";
    }

    //cleanup cURL
    curl_easy_cleanup(curl);
}

/*
 * The promptNewGameInfo function takes in the JSONParser object and prompts the user for information on adding a new game to the existing JSON data.
 * The information is then passed to the JSONParser's function to add new JSON data. 
 * The information for the new game is then printed to the console.
 */
void promptNewGameInfo(JSONParser& json)
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

    json.addNewJSON(gameDate, visTeamName, homeTeamName, isNeutral, isFinal);
    std::cout << "\nThank you. The results for each team have been generated based on the game date. See Below: \n";
    std::cout << json.getLatestJSON() << std::endl;

}

/*
 * This function determines if the user wants to print the JSON data to a file (1), query the JSON data (2), or add new JSON data (3).
 * Additional information is prompted to the user depending on their selection and the associated task is executed by calling the JSONParser and 
 * FileHandler functions.
 */
void promptAllOptions(int selectedOption, JSONParser& jsonParser, FileHandler &fileHandler)
{
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
        std::string gameDate;

        std::cout << "Please enter the game date as YYYY-MM-DD format: ";
        std::cin >> gameDate;

        std::string queriedData = jsonParser.queryJSON(gameDate);
        std::cout << queriedData << std::endl;

    }
    else if (selectedOption == 3)
    {
        promptNewGameInfo(jsonParser);
    }
}


int main()
{
    std::string webLink;
    std::string response;
    int selectedOption = 0;
    int exception;

    //provide a welcome message to the user and prompt the user to enter a web URL
    std::cout << "Welcome to the JSON Parser!" << std::endl;
    std::cout << "Please enter the web link to the JSON data: ";
    getline(std::cin, webLink);

    try 
    {
        connectToWebAPI(webLink, response);
    }
    catch (const char* message)
    {
        std::cerr << message << std::endl;
        return -1;
    }
 
    JSONParser json(response.c_str());
    FileHandler fileHandler;

    do
    {
        //ask user what they want to do: generate, retrieve, or read the JSON data, or or enter -1 to exit the program
        std::cout << "\n\nEnter 1, 2, or 3 for the options below, OR enter -1 to quit this program: \n";
        std::cout << "1. Read all the JSON Data to a file.\n";
        std::cout << "2. Query the JSON object and retrieve its associated data.\n";
        std::cout << "3. Add new JSON data based on a game previously played.\n";
        std::cin >> selectedOption;
        std::cout << std::endl;

        try 
        {
            promptAllOptions(selectedOption, json, fileHandler);
        }
        catch (const char* message)
        {
            std::cerr << message << std::endl;
            selectedOption = 0;
        }

    } while (selectedOption != -1);

}



