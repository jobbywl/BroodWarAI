#include "Schedulable.h"


Schedulable::Schedulable()
{
}


Schedulable::~Schedulable()
{
}


bool Schedulable::operator>(const Schedulable& b)
{
	if (this->priority > b.priority)
		return true;
	return false;
}

bool Schedulable::operator<(const Schedulable& b)
{
	if (this->priority > b.priority)
		return false;
	return true;
}

bool Schedulable::operator==(const Schedulable& b)
{
	if (this->priority == b.priority)
		return true;
	return false;
}

void Schedulable::setPriority(int prio)
{
	priority = prio;
}