#include "WorkerUnit.h"

using namespace Worker;

WorkerUnit::WorkerUnit(BWAPI::Unit u)
:mp_unit(u)
, mp_resource(NULL)
{
}


WorkerUnit::~WorkerUnit()
{
}

BWAPI::Unit WorkerUnit::setResource(BWAPI::Unit res)
{
	BWAPI::Unit temp = mp_resource;
	mp_resource = res;
	return temp;
}

BWAPI::Unit WorkerUnit::getResource()
{
	return mp_resource;
}

bool WorkerUnit::isMineralGatherer()
{
	if (mp_resource == NULL)
		return false;
	return mp_resource->getType().isMineralField();
}

bool WorkerUnit::isGasGatherer()
{
	if (mp_resource == NULL)
		return false;
	return mp_resource->getType().isRefinery();
}

bool WorkerUnit::isBuilder()
{
	//not yey implemented always returns false
	return false;
}

bool WorkerUnit::isAlive()
{
	if (mp_resource)
		return false;
	return mp_unit->exists();
}

bool WorkerUnit::isIdle()
{
	if (mp_resource)
		return false;
	return mp_unit->isIdle();
}

BWAPI::Unit WorkerUnit::getUnitInterface()
{
	return mp_unit;
}

void WorkerUnit::gather()
{
	mp_unit->gather(mp_resource);
	//reset delivered flag
	delivered = false;
}

void WorkerUnit::move(BWAPI::Position pos)
{
	mp_unit->rightClick(pos);
}


void WorkerUnit::Update()
{
	//This gets called on every frame, it also contains a pointer to the class that called it
	
	if (isMineralGatherer())
	{		
		if (wasCarrying && !mp_unit->isCarryingMinerals())
		{
			delivered = true;
			//for debug issue a stop command
			mp_unit->stop();
		}
			
		drawInfo();
		wasCarrying = mp_unit->isCarryingMinerals();
	}
}

bool WorkerUnit::deliveredMinerals()
{
	return delivered;
}

void WorkerUnit::drawInfo()
{
	BWAPI::Broodwar->drawLineMap(mp_unit->getPosition(), mp_resource->getPosition(), BWAPI::Colors::Red);

	BWAPI::Broodwar->drawBoxMap(mp_unit->getLeft(), mp_unit->getTop(), mp_unit->getRight(), mp_unit->getBottom(), BWAPI::Colors::Red);

	BWAPI::Broodwar->drawTextMap(mp_unit->getPosition(), "%d", mp_unit->getID());
}