#include "WorkerManager.h"
#include "MineralLock.h"
using namespace Worker;

WorkerManager::WorkerManager()
:mp_basesList(NULL)
{
	mp_basesList = new std::unordered_map<int, MiningBase*>;
	//Always start with mineral lock
	start = FrameCounter::getInstance().CountedFrames();
}


WorkerManager::~WorkerManager()
{
	if (mp_basesList != NULL)
		delete mp_basesList;
}

//Adds a base to the list without workers
void WorkerManager::addBase(BWAPI::Unit base)
{
	if (mp_minerals.size() == 0)
	{
		//Create the mineralList for the map
		mp_minerals = BWAPI::Broodwar->getMinerals();
	}

	std::clog << "Adding a base" << std::endl;

	mp_basesList->emplace(base->getID(), new MiningBase(base));

	mp_basesList->at(base->getID())->setQueue();
}

//Adds a base to the list with a list of workers
void WorkerManager::addBase(BWAPI::Unit base, BWAPI::Unitset worker)
{
	if (mp_minerals.size() == 0)
	{
		//Create the mineralList for the map
		mp_minerals = BWAPI::Broodwar->getMinerals();
	}

	std::clog << "Adding a base with worker set" << std::endl;

	mp_basesList->emplace(base->getID(), new MiningBase(base));

	addWorkerToBase(base, worker);
	mp_basesList->at(base->getID())->setQueue();
}

//Searches in the list for the pointer of the base, Then adds the worker to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, BWAPI::Unit worker)
{
	std::clog << "Adding worker to base" << std::endl;
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{

		if ((*i).first == base->getID())
		{
			(*i).second->addWorker(worker);
			break;
		}
	}
}

//Searches in the list for the pointer of the base, Then adds the workerlist to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, BWAPI::Unitset worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i).first == base->getID())
		{
			(*i).second->addWorkerSet(worker);
			break;
		}
	}
}


void WorkerManager::checkWorkers()
{
	//This gets called every frame
	//If Unit is still worker and exist
	for (auto base : *mp_basesList)
	{
		for (auto w : (base.second->getWorkerList()))
		{
			if (!(w->getType().isWorker() || w->exists()))
			{
				base.second->remWorker(w);
			}
		}
		base.second->drawInfo();
		base.second->checkWorkers();
	}
	//exeecute


}


//This function returns the need of making an expansion based on minerals and gas left
float WorkerManager::needExpansion()
{
	return 0;
}

void WorkerManager::drawTimers()
{


}

double WorkerManager::CalcTravelTime(BWAPI::Unit a, BWAPI::Unit b)
{
	double speed = a->getType().topSpeed();
	double accel = a->getType().acceleration();
	double haltd = a->getType().haltDistance();

	double distance = a->getDistance(b);

	//vt-vi / a = t
	double timeTillMaxSpeed = speed / accel;

	double accelDistance = 0.5 * accel * pow(timeTillMaxSpeed, 2);

	if (accelDistance > distance)
		return timeTillMaxSpeed;

	return timeTillMaxSpeed + distance / speed;
}