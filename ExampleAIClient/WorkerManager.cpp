#include "WorkerManager.h"
using namespace Worker;

WorkerManager::WorkerManager()
:mp_basesList(NULL)
, mp_queue(NULL)
, mp_mineralLock(NULL)
, mp_coopPathfinding(NULL)
{
	mp_basesList = new std::list<MiningBase*>;
	//Always start with mineral lock
	setqueueSystem(true);
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
	addBase(base, NULL);
}

//Adds a base to the list with a list of workers
void WorkerManager::addBase(BWAPI::Unit base, std::list<BWAPI::Unit>* worker)
{
	std::clog << "Adding a base" << std::endl;

	BWAPI::Unitset mineralset = BWAPI::Broodwar->getMinerals();
	//create a list on the heap
	std::list<BWAPI::Unit> *tempMinerals = new std::list<BWAPI::Unit>;
	//Generate the list for each base with its minerals
	for (auto &m : mineralset)
	{
		//only add local minerals, Distance is just a guess
		std::cout << base->getDistance(m) << std::endl;
		if (base->getDistance(m) < 200)
			tempMinerals->push_back((BWAPI::Unit)m);
	}

	BWAPI::Unitset geyserset = BWAPI::Broodwar->getGeysers();
	//create a list on the heap
	std::list<BWAPI::Unit> *tempGeysers = new std::list<BWAPI::Unit>;
	//Generate the list for each base with its geysers
	for (auto &m : geyserset)
	{
		//only add local geysers, Distance is just a guess
		std::cout << base->getDistance(m) << std::endl;
		if (base->getDistance(m) < 200)
			tempGeysers->push_back((BWAPI::Unit)m);
	}


	MiningBase *temp = new MiningBase{ base, new std::list<BWAPI::Unit>, tempMinerals, tempGeysers };

	mp_basesList->push_back(temp);

	addWorkerToBase(base, worker);
}

//Searches in the list for the pointer of the base, Then adds the worker to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, BWAPI::Unit worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{

		if ((*i)->depot->getID() == base->getID())
		{
			mp_queue->previousWorkerState.emplace(worker, false);
			(*i)->workers->push_back(worker);
			break;
		}
	}
}

//Searches in the list for the pointer of the base, Then adds the workerlist to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, std::list<BWAPI::Unit>* worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i)->depot->getID() == base->getID())
		{
			(*i)->workers->splice((*i)->workers->end(), *worker);
			for (auto w : *(*i)->workers)
			{
				mp_queue->previousWorkerState.emplace(w, false);
			}
			break;
		}
	}	
}


//Mineral lock cant be combined so it will turn all the other micro methods of
void WorkerManager::setMineralLock()
{

	if (mp_coopPathfinding)
	{
		delete mp_coopPathfinding;
		mp_coopPathfinding = NULL;
	}

	if (mp_queue)
	{
		delete mp_queue;
		mp_queue = NULL;
	}
	mp_mineralLock = new MineralLockData;
}

//This can be combined
void WorkerManager::setqueueSystem(bool temp)
{
	if (temp)
	{
		if (mp_mineralLock)
		{
			delete mp_mineralLock;
			mp_mineralLock = NULL;
		}

		//Make sure there are enough places for the previous states
		for (auto baseIterator : *mp_basesList)
		{
			for (auto w : *(baseIterator->workers))
				mp_queue->previousWorkerState.emplace(w, false);
		}

		mp_queue = new QueueData;
	}
	else
	{
		if (mp_queue)
		{
			delete mp_queue;
			mp_queue = NULL;
		}

		if (!getcoopPathfinding())
		{
			//If no other is defined set mineral lock
			setMineralLock();
		}
	}

}

//This can be combined
void WorkerManager::setcoopPathfinding(bool temp)
{
	if (temp)
	{
		if (mp_mineralLock)
		{
			delete mp_mineralLock;
			mp_mineralLock = NULL;
		}
		mp_coopPathfinding = new CoopPathfindingData;
	}
	else
	{
		if (mp_coopPathfinding)
		{
			delete mp_coopPathfinding;
			mp_coopPathfinding = NULL;
		}

		if (!getqueueSystem())
		{
			//If no other is defined set mineral lock
			setMineralLock();
		}
	}

}

bool WorkerManager::getMineralLock()
{
	if (mp_mineralLock)
		return true;
	return false;
}

bool WorkerManager::getqueueSystem()
{
	if (mp_queue)
		return true;
	return false;
}

bool WorkerManager::getcoopPathfinding()
{
	if (mp_coopPathfinding)
		return true;
	return false;
}

void WorkerManager::checkWorkers()
{
	if (getMineralLock())
		mineralLock();
	else if (getqueueSystem())
		queueSystem();
	//exeecute
}

//Bijhouden welke worker op welke mineral patch zit in vector
//Check of de worker idle is en zo ja koppel worker aan minerals
//Stuur worker als die passief is naar de minerals
void WorkerManager::mineralLock()
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		//for each worker per base micro them
		//make sure the worker is on the same mineral field
		//(*i)->workers
		//send each worker to the mineral field that is closest to it

	}
}

void WorkerManager::queueSystem()
{
	drawTimers();


	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		//for each worker per base micro them
		//Send each worker to the closest available mineral patch, keeping in mind gather time and travel time

		//Create a list with minerals based on the distance from the base
		std::list<BWAPI::Unit> listCopy(*(*i)->minerals);
		while (mp_queue->mineralMap.size() != (*i)->minerals->size())
		{
			BWAPI::Unit Closestmineral = listCopy.front();


			for (auto mineral = listCopy.begin(); mineral != listCopy.end(); mineral++)
			{
				if (Closestmineral->getDistance((*i)->depot) < (*mineral)->getDistance((*i)->depot))
					Closestmineral = (*mineral);
			}
			listCopy.remove(Closestmineral);

			mp_queue->mineralTimer.push_back(0.0);
			mp_queue->mineralVector.push_back(Closestmineral);
			mp_queue->mineralMap.emplace(Closestmineral, mp_queue->mineralMap.size());
		}

		//Update the mineral timers.

		for (auto &i : mp_queue->mineralTimer)
		{
			if (i > 0.0)
				i -= FrameCounter::getInstance().CountedFrames() - start;
			else
				i = 0;
		}
		start = FrameCounter::getInstance().CountedFrames();


		//iterate through all its workers
		for (auto w : (*(*i)->workers))
		{
			//if the probe was carrying minerals in the previous frame
			//Or the worker is idle
			if ((!w->isCarryingMinerals() && mp_queue->previousWorkerState.find(w)->second) || w->isIdle())
			{
				//if worker is idle and not gathering anything


				for (int i = 0; i < mp_queue->mineralTimer.size(); i++)
				{
					//Calculate the time it takes to travel to the mineral. If it is less then a worker is busy at the closest mineral patch then send it to the minereal patch.

					if (CalcTravelTime((BWAPI::Unit)w, mp_queue->mineralVector[i]) > mp_queue->mineralTimer[i])
					{
						mp_queue->mineralTimer[i] = CalcTravelTime((BWAPI::Unit)w, mp_queue->mineralVector[i]) * 2 + mp_queue->gatherTime;
						w->rightClick(mp_queue->mineralVector[i]);

						//save previous state
						mp_queue->previousWorkerState.find(w)->second = w->isCarryingMinerals();
						break;
					}
				}
			}
			//save previous state
			mp_queue->previousWorkerState.find(w)->second = w->isCarryingMinerals();
		}
	}
}


// 0.5av^2
//--------
//  0.5a^3  = s
//




void WorkerManager::coopPathfinding()
{

}

//This function returns the need of making an expansion based on minerals and gas left
float WorkerManager::needExpansion()
{
	return 0;
}

void WorkerManager::drawTimers()
{
	for (int i = 0; i < mp_queue->mineralVector.size(); i++)
	{

		BWAPI::Broodwar->drawTextMap(mp_queue->mineralVector[i]->getPosition(), "%f", mp_queue->mineralTimer[i], BWAPI::Text::Red);
	}

	for (auto &w : *(mp_basesList->front()->workers))
	{
		BWAPI::Broodwar->drawBoxMap(w->getLeft(), w->getTop(), w->getRight(), w->getBottom(), BWAPI::Color(0xff, 0x00, 0x00), true);
		BWAPI::Broodwar->drawLineMap(w->getPosition(), w->getLastCommand().getTargetPosition(), BWAPI::Color(0xff, 0x00, 0x00));
	}

}

double WorkerManager::CalcTravelTime(BWAPI::Unit a, BWAPI::Unit b)
{
	double traveltime;

	double speed = a->getType().topSpeed();
	double accel = a->getType().acceleration();
	double haltd = a->getType().haltDistance();

	double distance = a->getDistance(b);

	traveltime = distance / speed;




	return traveltime;
}