#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

CSVReader::CSVReader(std::string temp)
{
	CSVFile = new std::ifstream(temp, std::ios::in);
	if (CSVFile->is_open())
	{
		std::cout << "File Opened";
	}
}


CSVReader::~CSVReader()
{
	delete CSVFile;
}

std::list<std::string>* CSVReader::readLine()
{
	std::list<std::string>* temp = new std::list<std::string>;
	
	std::stringstream stream;
	std::string line;
	if (!std::getline(*CSVFile, line))
		return NULL;

	if (line[0] == ('#'))
	{
		if (!std::getline(*CSVFile, line))
			return NULL;
	}
	stream << line;
	std::string item;
	while (std::getline(stream, item, ';'))
	{
		temp->push_back(item);
	}
	return temp;
	
}