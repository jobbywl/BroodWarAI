#include "Queue.h"

using namespace Worker;

Queue::Queue(MiningBase *base)
: mp_minerals(NULL)
, mp_base(base)
{
	mp_minerals = new std::multimap<int, BWAPI::Unit>;
	mp_mineralTimer = new std::unordered_map<int, int>;
	//Create a local ordered list for the mineral fields
	for (auto m : mp_base->getResourceContainers())
	{
		if (m->getType().isMineralField())
		{
			mp_minerals->emplace(mp_base->getResourceDepot()->getDistance(m), m);
			//add every base and set every second value to 0 which means free
			mp_mineralTimer->emplace(m->getID(), 0);
		}
			
	}
}


Queue::~Queue()
{
	delete mp_minerals;
	delete mp_mineralTimer;
	mp_minerals = NULL;
	mp_mineralTimer = NULL;
}

void Queue::execute()
{
	//update the timers
	for (auto m : *mp_mineralTimer)
	{
		if (m.second > 0)
			m.second--;
	}


	for (auto worker : *(mp_base->getWorkerUnitList()))
	{
		//If worker is not a gas gatherer or a builder and idle and the base is not saturated
		if ((!worker.second->isGasGatherer()) && (!worker.second->isBuilder()) && worker.second->isIdle() && !mp_base->isSaturated())
		{
			//if the worker is a new unknown worker

			//iterate through all the minerals
			for (auto m : *mp_minerals)
			{
				//with the id of the mineral lookup the timer value if this is less then the traveltime set the resource and change the timer to a big value
				if (mp_mineralTimer->at(m.second->getID()) < calcTravelTime(worker.second->getUnitInterface(), m.second))
				{
					worker.second->setResource(m.second);
					mp_mineralTimer->at(m.second->getID()) = 10000;
					worker.second->gather();
					break;
				}
			}
		}
		//if worker is a mineralgatherer and not carrying minerals
		else if (worker.second->isMineralGatherer() && !worker.second->getUnitInterface()->isCarryingMinerals())
		{
			//if the worker just turned in its minerals
			
			int id1 = worker.second->getResource()->getID();


			BWAPI::Unit target = worker.second->getUnitInterface()->getTarget();
			if (target == NULL)
			{
				worker.second->gather();
				break;
			}
				

			int id2 = worker.second->getUnitInterface()->getTarget()->getID();

			if (id1 != id2)
				worker.second->gather();
		}

		//if worker stopped moving and is gathering minerals start the timer
		if (worker.second->isMineralGatherer() && !worker.second->getUnitInterface()->isMoving() && worker.second->getUnitInterface()->isGatheringMinerals())
		{
			if (worker.second->getResource() != (BWAPI::Unit)1)
			{
				mp_mineralTimer->at(worker.second->getResource()->getID()) = 90;
				//set resource to a special value with which gather is idle but it still returns as an mineral gatherer
				worker.second->setResource((BWAPI::Unit)1);
			}
				
		}

		/*
		if (worker.second->deliveredMinerals())
			worker.second->setResource((BWAPI::Unit)1);
			*/
	}
}

bool Queue::isQueue()
{
	return true;
}