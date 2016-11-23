#pragma once
#include <string>
#include <list>
#include <fstream>

class CSVReader
{
public:
	CSVReader(std::string filename);
	~CSVReader();

	std::list<std::string>* readLine();
private:
	std::ifstream *CSVFile;
};

