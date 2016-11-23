#pragma once
#include <list>
#include <string>

class Unit
{
public:
	Unit(std::list<std::string>*);
	virtual ~Unit();
private:
	//stats
	std::string name;
	//current
	int Amount;
};

