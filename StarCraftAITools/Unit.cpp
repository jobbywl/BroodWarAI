#include "Unit.h"


Unit::Unit(std::list<std::string> *temp)
:Amount(0)
{
	temp->pop_front();
	auto i = temp->begin();
	name = *i;
}


Unit::~Unit()
{
}
