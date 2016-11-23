#include "Terran.h"
#include <Windows.h>
#include "CSVReader.h"

Terran::Terran()
{
	mp_Buildings = new std::map<std::string, Unit*>;
	mp_Units = new std::map<std::string, Unit*>;
	mp_Upgrades = new std::map<std::string, Unit*>;
	
	CSVReader reader("terran.csv");

	//Read first line from csv
	std::list<std::string> *temp = NULL;

	temp = reader.readLine();
	while (temp != NULL)
	{
		
		auto i = temp->begin();
		if (*i == "Unit")
		{
			i++;
			(*mp_Units)[*i] = new Unit(temp);
		}
		else if (*i == "Building")
		{
			i++;
			//(*mp_Buildings)[*i] = new Unit();
		}
		else if (*i == "Upgrade")
		{
			i++;
			//(*mp_Upgrades)[*i] = new Unit();
		}
		else
		{
			//error invalid file
		}
		//delete the line
		delete temp;
		temp = reader.readLine();
	}
}


Terran::~Terran()
{
	delete mp_Buildings;
	delete mp_Units;
	delete mp_Upgrades;
}

void AddUnit(TerranUnits)
{

}

void AddBuilding(TerranBuildings)
{

}

void AddUpgrade(TerranUpgrades)
{

}