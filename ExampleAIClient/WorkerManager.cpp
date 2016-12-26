#include "WorkerManager.h"
using namespace Worker;

WorkerManager::WorkerManager()
:mp_basesList(NULL)
, queuesystem(true)
, cooppathfinding(false)
, minerallock(false)
{
	mp_basesList = new std::unordered_map<BWAPI::Unit, MiningBase*>;
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
		if (base->getDistance(m) < 300)
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


	MiningBase *temp = new MiningBase{ new std::list<BWAPI::Unit>, tempMinerals, tempGeysers };
	temp->mp_coopPathfinding = new CoopPathfindingData;
	temp->mp_mineralLock = new MineralLockData;
	temp->mp_queue = new QueueData;

	mp_basesList->emplace(base, temp);

	addWorkerToBase(base, worker);
}

//Searches in the list for the pointer of the base, Then adds the worker to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, BWAPI::Unit worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{

		if ((*i).first->getID() == base->getID())
		{
			(*i).second->mp_queue->previousWorkerState.emplace(worker, false);
			(*i).second->workers->push_back(worker);
			break;
		}
	}
}

//Searches in the list for the pointer of the base, Then adds the workerlist to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit base, std::list<BWAPI::Unit>* worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i).first->getID() == base->getID())
		{
			if (worker != NULL)
			{
				(*i).second->workers->splice((*i).second->workers->end(), *worker);
				for (auto w : *(*i).second->workers)
				{
					(*i).second->mp_queue->previousWorkerState.emplace(w, false);
				}
			}

			break;
		}
	}
}


//Mineral lock cant be combined so it will turn all the other micro methods of
void WorkerManager::setMineralLock()
{
	for (auto i : *mp_basesList)
	{
		
		if ((i).second->mp_coopPathfinding)
		{
			delete (i).second->mp_coopPathfinding;
			(i).second->mp_coopPathfinding = NULL;
		}

		if ((i).second->mp_queue)
		{
			delete (i).second->mp_queue;
			(i).second->mp_queue = NULL;
		}
		(i).second->mp_mineralLock = new MineralLockData;
	}
}


//This can be combined
void WorkerManager::setqueueSystem(bool temp)
{
	for (auto i : *mp_basesList)
	{
		if (temp)
		{
			if ((i).second->mp_mineralLock)
			{
				delete (i).second->mp_mineralLock;
				(i).second->mp_mineralLock = NULL;
			}

			//Make sure there are enough places for the previous states
			for (auto baseIterator : *mp_basesList)
			{
				for (auto w : *(baseIterator.second->workers))
					(i).second->mp_queue->previousWorkerState.emplace(w, false);
			}

			(i).second->mp_queue = new QueueData;
		}
		else
		{
			if ((i).second->mp_queue)
			{
				delete (i).second->mp_queue;
				(i).second->mp_queue = NULL;
			}

			if (!getcoopPathfinding())
			{
				//If no other is defined set mineral lock
				setMineralLock();
			}
		}
	}
}

//This can be combined
void WorkerManager::setcoopPathfinding(bool temp)
{
	for (auto i : *mp_basesList)
	{
		if (temp)
		{
			if ((i).second->mp_mineralLock)
			{
				delete (i).second->mp_mineralLock;
				(i).second->mp_mineralLock = NULL;
			}
			(i).second->mp_coopPathfinding = new CoopPathfindingData;
		}
		else
		{
			if ((i).second->mp_coopPathfinding)
			{
				delete (i).second->mp_coopPathfinding;
				(i).second->mp_coopPathfinding = NULL;
			}

			if (!getqueueSystem())
			{
				//If no other is defined set mineral lock
				setMineralLock();
			}
		}
	}
}

bool WorkerManager::getMineralLock()
{
	if (minerallock)
		return true;
	return false;
}

bool WorkerManager::getqueueSystem()
{
	if (queuesystem)
		return true;
	return false;
}

bool WorkerManager::getcoopPathfinding()
{
	if (cooppathfinding)
		return true;
	return false;
}

void WorkerManager::checkWorkers()
{
	//If Unit is still worker and exist
	for (auto base : *mp_basesList)
	{
		for (auto w : *(base.second->workers))
		{
			if (!(w->getType().isWorker() || w->exists()))
			{
				base.second->workers->remove(w);
			}
		}

	}

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
		std::list<BWAPI::Unit> listCopy(*(*i).second->minerals);
		while (i->second->mp_queue->mineralMap.size() != (*i).second->minerals->size())
		{
			BWAPI::Unit Closestmineral = listCopy.front();


			for (auto mineral = listCopy.begin(); mineral != listCopy.end(); mineral++)
			{
				if (Closestmineral->getDistance((*i).first) > (*mineral)->getDistance((*i).first))
					Closestmineral = (*mineral);
			}
			listCopy.remove(Closestmineral);

			i->second->mp_queue->mineralTimer.push_back(0.0);
			i->second->mp_queue->mineralVector.push_back(Closestmineral);
			i->second->mp_queue->mineralMap.emplace(Closestmineral, i->second->mp_queue->mineralMap.size());

			//Update the mineral timers.

			
		}
		

		for (auto &j : i->second->mp_queue->mineralTimer)
		{
			if (j > 0.0)
				j -= FrameCounter::getInstance().CountedFrames() - start;
			else
				j = 0;
		}
		start = FrameCounter::getInstance().CountedFrames();
		//iterate through all its workers
		for (auto w : (*(*i).second->workers))
		{
			//if the probe was carrying minerals in the previous frame
			//Or the worker is idle
			if ((!w->isCarryingMinerals() && i->second->mp_queue->previousWorkerState.find(w)->second) || w->isIdle())
			{
				//if worker is idle and not gathering anything


				for (int j = 0; j < i->second->mp_queue->mineralTimer.size(); j++)
				{
					//Calculate the time it takes to travel to the mineral. If it is less then a worker is busy at the closest mineral patch then send it to the minereal patch.

					if (CalcTravelTime((BWAPI::Unit)w, i->second->mp_queue->mineralVector[j]) > i->second->mp_queue->mineralTimer[j])
					{
						i->second->mp_queue->mineralTimer[j] = CalcTravelTime((BWAPI::Unit)w, i->second->mp_queue->mineralVector[j]) + i->second->mp_queue->gatherTime;
						w->rightClick(i->second->mp_queue->mineralVector[j]);

						//save previous state
						i->second->mp_queue->previousWorkerState.find(w)->second = w->isCarryingMinerals();
						break;
					}
					else
					{
						//No mineral patch so idle
						w->stop();
					}
				}
			}
			//save previous state
			i->second->mp_queue->previousWorkerState.find(w)->second = w->isCarryingMinerals();
		}
	}
}

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
	for (auto base : *mp_basesList)
	{
		for (int i = 0; i < base.second->mp_queue->mineralVector.size(); i++)
		{

			BWAPI::Broodwar->drawTextMap(base.second->mp_queue->mineralVector[i]->getPosition(), "%f", base.second->mp_queue->mineralTimer[i], BWAPI::Text::Red);
		}
	}
	for (auto &w : *((*mp_basesList->begin()).second->workers))
	{
		BWAPI::Broodwar->drawBoxMap(w->getLeft(), w->getTop(), w->getRight(), w->getBottom(), BWAPI::Color(0xff, 0x00, 0x00), true);
		//BWAPI::Broodwar->drawLineMap(w->getPosition(), w->getLastCommand().getTargetPosition(), BWAPI::Color(0xff, 0x00, 0x00));
	}

	for (auto base : *mp_basesList)
	{
		BWAPI::Broodwar->drawTextMap(base.first->getPosition(), "%d", base.first->getID(), BWAPI::Text::Red);
	}

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