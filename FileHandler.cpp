#include "FileHandler.h"


FileHandler::FileHandler(const std::string& dirName)
{
	//initialize variables
	setDirName(dirName);
	//setOutputData(data);
	fileName = "ParsedJson.txt";
}

bool FileHandler::directoryExist()
{
	return std::filesystem::is_directory(getDirectory());
}

void FileHandler::write(const std::string& outputData)
{
	clear();

	file.open(getFullPath(), std::ios::out | std::ios::app);
	file << outputData << std::endl;
	file.close();

}

void FileHandler::clear()
{
	file.open(getFullPath(), std::ios::out);
	file.close();
}


void FileHandler::setDirName(const std::string& dirName)
{
	directory = dirName;
}

//void FileHandler::setOutputData(const std::string& data)
//{
//	outputData = data;
//}

std::filesystem::path FileHandler::getFullPath()
{
	std::filesystem::path path(getDirectory() + "\\" + fileName);
	return path;
}

//std::string FileHandler::getData()
//{
//	return this->outputData;
//}

std::string FileHandler::getDirectory()
{
	return this->directory;
}

