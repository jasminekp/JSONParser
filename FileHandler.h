#pragma once

#include <fstream>
#include <filesystem>

class FileHandler
{

public:

	//constructor
	FileHandler();

	//modifying functions
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

