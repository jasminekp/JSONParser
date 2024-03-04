/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #1
 * 2/4/2024
 *
 * This FileHandler.h header file contains the private and public data members/functions for the FileHandler class.
 **/


#pragma once

#include <fstream>
#include <filesystem>

class FileHandler
{

public:

	//constructor
	FileHandler();

	//functions that modify the file and check directory validity
	bool directoryExist(const std::string& dirName);
	void write(const std::string& outputData);
	void clear();
	
	//setters
	void setDirName(const std::string& dirName);
	void setFileName(std::string fileName);
	
	//getters
	std::filesystem::path getFullPath();
	std::string getDirectory();
	std::string getFileName();


private:
	std::fstream file;
	std::string directory;
	std::filesystem::path fullPath;
	std::string fileName;

};

