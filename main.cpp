#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "JsonProcessor.h"
#include "rapidjson/document.h"

int main ()

{

 // read the JSON file
 std::ifstream file("team49ers_season2020.txt");
    if (!file.is_open()) 
{
        std::cerr << "Error opening file!" << std::endl; 
        return 1; 
}

//create an instance of a stringstream to read the JSON file content into a string

std::stringstream buffer; 
buffer << file.rdbuf();

// Extract the content as a string

std::string jsonString = buffer.str();

// Parse the JSON string using RapidJSON

rapidjson::Document document; 
document.Parse(jsonString.c_str());

JsonProcessor jsonprocessor(jsonString.c_str());

return 0; 

}