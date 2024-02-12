#include "FileHandler.h"

FileHandler::FileHandler()
{
	//set the name of the output file that will contain the parsed JSON data
	setFileName("ParsedJson.txt");
}

/*
 * The directoryExist function returns a boolean indicating whether the directory is a valid path entered by the user 
 */
bool FileHandler::directoryExist(const std::string& dirName)
{
	setDirName(dirName);

	if (!std::filesystem::is_directory(getDirectory()))
	{
		throw "\nERROR: Directory is not valid.";
	}
	
	return true;
}

/*
 * The write function writes the contents of the data to a file within a specified file path
 * To ensure that the file is not appended to in each write, the file contents are cleared by calling the clear function
 */
void FileHandler::write(const std::string& outputData)
{
	clear();

	file.open(getFullPath(), std::ios::out | std::ios::app);
	file << outputData << std::endl;
	file.close();

}

/*
 * The clear function opens the file and writes the contents 
 */
void FileHandler::clear()
{
	file.open(getFullPath(), std::ios::out);
	file.close();
}


void FileHandler::setDirName(const std::string& dirName)
{
	directory = dirName;
}

void FileHandler::setFileName(std::string fileName)
{
	this->fileName = "ParsedJson.txt";
}

std::filesystem::path FileHandler::getFullPath()
{
	std::filesystem::path path(getDirectory() + "\\" + fileName);
	return path;
}

std::string FileHandler::getFileName()
{
	return this->fileName;
}

std::string FileHandler::getDirectory()
{
	return this->directory;
}

