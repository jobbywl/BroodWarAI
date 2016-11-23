#include "Unit.h"


Unit::Unit(std::list<std::string> *temp)
:Amount(0)
{
	groundAttackType.explosive = false;
	groundAttackType.concussive = false;
	groundAttackType.splash = false;

	airAttackType.explosive = false;
	airAttackType.concussive = false;
	airAttackType.splash = false;

	temp->pop_front();
	auto i = temp->begin();
	name = *i++;
	
	if ((*i).find('S') != std::string::npos)
	{
		size = S;
	}
	else if ((*i).find('M') != std::string::npos)
	{
		size = M;
	}
	else if ((*i).find('L') != std::string::npos)
	{
		size = L;
	}
	i++;

	supply = atoi((*i++).c_str());
	minerals = atoi((*i++).c_str());
	gas = atoi((*i++).c_str());
	armor = atoi((*i++).c_str());
	hp = atoi((*i++).c_str());
	groundAttack = atoi((*i).c_str());
	
	if ((*i).find('e') != std::string::npos   )
	{
		groundAttackType.explosive = true;
	}
	if ((*i).find('c') != std::string::npos)
	{
		groundAttackType.concussive = true;
	}
	if ((*i).find('s') != std::string::npos)
	{
		groundAttackType.splash = true;
	}
	i++;

	airAttack = atoi((*i).c_str());
	
	if ((*i).find('e') != std::string::npos)
	{
		airAttackType.explosive = true;
	}
	if ((*i).find('c') != std::string::npos)
	{
		airAttackType.concussive = true;
	}
	if ((*i).find('s') != std::string::npos)
	{
		airAttackType.splash = true;
	}
	i++;

	attackSpeed = atoi((*i++).c_str());
	range = atoi((*i++).c_str());
	attackMod = atoi((*i++).c_str());
	sight = atoi((*i++).c_str());
	notes = *i++;
	buildTime = atoi((*i).c_str());
}


Unit::~Unit()
{
}
