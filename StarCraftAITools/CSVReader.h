#pragma once
#include <string>
#include <list>

class CSVReader
{
public:
	CSVReader(std::string filename);
	~CSVReader();

	std::list<std::string>* readLine();
};

