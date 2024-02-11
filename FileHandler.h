#pragma once

#include <fstream>
#include <filesystem>

class FileHandler
{

public:
	FileHandler(const std::string& dirName);
	bool directoryExist();
	void write(const std::string& outputData);
	void clear();
	
	void setDirName(const std::string& dirName);
	void setOutputData(const std::string& data);

	std::string getData();
	std::filesystem::path getFullPath();
	std::string getDirectory();


private:
	
	std::fstream file;
	//std::string outputData;
	std::string directory;
	std::filesystem::path fullPath;
	std::string fileName;
};

