/**
 * Jasmine Parmar, Kevin Osadiaye
 * CSE 681 - Software Modeling
 * Syracuse University - Professor Greg Wagner
 * Project #2
 * 3/3/2024
 *
 * This FileHandler.cpp file contains the implementation for the FileHandler class. The FileHandler manages all the file writing,
 * creating, opening, and closing of an output file that is specified within a set directory.
 **/

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
 * The clear function opens the file and writes empty contents before closing the file
 */
void FileHandler::clear()
{
	file.open(getFullPath(), std::ios::out);
	file.close();
}

/*
 * setDirName is the setter for the directory path passed in as a string
 */
void FileHandler::setDirName(const std::string& dirName)
{
	directory = dirName;
}

/*
 * setFileName is the setter for the output file name - always titled 'ParsedJson.txt'
 */
void FileHandler::setFileName(std::string fileName)
{
	this->fileName = "ParsedJson.txt";
}

/*
 * The getFullPath returns the full path of directory to write the file to - including the file name
 */
std::filesystem::path FileHandler::getFullPath()
{
	std::filesystem::path path(getDirectory() + "\\" + fileName);
	return path;
}

/*
 * The getFileName returns the file name (ParsedJson.txt)
 */
std::string FileHandler::getFileName()
{
	return this->fileName;
}

/*
 * The getFileName returns the directory name
 */
std::string FileHandler::getDirectory()
{
	return this->directory;
}

