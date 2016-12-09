#include "WorkerManager.h"
using namespace Worker;

WorkerManager::WorkerManager()
:mp_basesList(NULL)
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
	addBase(base, 0);
}

//Adds a base to the list with a list of workers
void WorkerManager::addBase(BWAPI::Unit *base, std::list<BWAPI::Unit*> *worker)
{
	MiningBase *temp = new MiningBase{ base, 0 };
	temp->workers = worker;

	mp_basesList->push_back(temp);
}

//Searches in the list for the pointer of the base, Then adds the worker to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit *base, BWAPI::Unit *worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i)->depot == base)
		{
			(*i)->workers->push_back(worker);
			break;
		}
	}
}

//Searches in the list for the pointer of the base, Then adds the workerlist to the list
void WorkerManager::addWorkerToBase(BWAPI::Unit *base, std::list<BWAPI::Unit*>*worker)
{
	for (auto i = mp_basesList->begin(); i != mp_basesList->end(); i++)
	{
		if ((*i)->depot == base)
		{
			std::list<BWAPI::Unit*> *temp = (*i)->workers;
			temp->splice(temp->end(), *worker);
			break;
		}
	}
}

void WorkerManager::checkWorkers()
{

}