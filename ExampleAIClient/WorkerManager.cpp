#include "WorkerManager.h"
using namespace Worker;

WorkerManager::WorkerManager()
:mp_basesList(NULL)
, m_mineralLock(false)
, m_queueSystem(false)
, m_coopPathfinding(false)
{
	mp_basesList = new std::list<MiningBase*>;
}


WorkerManager::~WorkerManager()
{
	if (mp_basesList != NULL)
		delete mp_basesList;
}

//Adds a base to the list without workers
void WorkerManager::addBase(BWAPI::Unit *base)
{
	addBase(base, NULL);
}

//Adds a base to the list with a list of workers
void WorkerManager::addBase(BWAPI::Unit *base, std::list<BWAPI::Unit>* worker)
{
	std::clog << "Added a base" << std::endl;
	MiningBase *temp = new MiningBase{ base, worker };

	mp_basesList->push_back(temp);
}

//Searches in the list for the pointer of the base, Then adds the worker to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit *base, BWAPI::Unit *worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i)->depot == base)
		{
			(*i)->workers->push_back(*worker);
			break;
		}
	}
}

//Searches in the list for the pointer of the base, Then adds the workerlist to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit *base, std::list<BWAPI::Unit>* worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i)->depot == base)
		{
			(*i)->workers->splice((*i)->workers->end(),*worker);
			break;
		}
	}
}


void WorkerManager::setMineralLock(bool temp)
{
	if (temp)
	{
		m_coopPathfinding = !temp;
		m_queueSystem = !temp;
	}
	m_mineralLock = temp;

}

void WorkerManager::setqueueSystem(bool temp)
{
	if (temp)
		m_mineralLock = !temp;
	m_queueSystem = temp;
}

void WorkerManager::setcoopPathfinding(bool temp)
{
	if (temp)
		m_mineralLock = !temp;
	m_coopPathfinding = temp;
}

bool WorkerManager::getMineralLock()
{
	return m_mineralLock;
}

bool WorkerManager::getqueueSystem()
{
	return m_queueSystem;
}

bool WorkerManager::getcoopPathfinding()
{
	return m_coopPathfinding;
}

void WorkerManager::checkWorkers()
{
	if (m_mineralLock)
	{
		mineralLock();
	}
}

void WorkerManager::mineralLock()
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		//for each worker per base micro them
		//make sure the worker is on the same mineral field
		//(*i)->workers
		//send each worker to the mineral field that is closest to it

		BWAPI::Unitset minerals = BWAPI::Broodwar->getMinerals();
		for (auto u = (*i)->workers->begin(); u != (*i)->workers->end();u++)
		{
			if ((*u)->getType().isWorker() && !((*u)->isCarryingMinerals() || (*u)->isGatheringMinerals()))
			{
				BWAPI::Unit closestMineral = nullptr;

				for (auto &m : minerals)
				{
					if (!closestMineral || (*u)->getDistance(m) < (*u)->getDistance(closestMineral))
						closestMineral = m;
				}
				if (closestMineral)
					(*u)->rightClick(closestMineral);
			}
		}
	}
}

void WorkerManager::queueSystem()
{

}

void WorkerManager::coopPathfinding()
{

}