#pragma once
#include <list>
#include <map>
#include "Unit.h"


class Race
{
public:
	Race(std::string);
	~Race();

	void AddUnit(std::string unit);
	void AddBuilding(std::string building);
	void AddUpgrade(std::string upgrade);

private:
	std::map<std::string,Unit*> *mp_Units;
	std::map<std::string, Unit*> *mp_Buildings;
	std::map<std::string, Unit*> *mp_Upgrades;
};

