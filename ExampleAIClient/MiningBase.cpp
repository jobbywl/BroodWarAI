#include "MiningBase.h"
#include "Queue.h"
#include "MineralLock.h"

using namespace Worker;

MiningBase::MiningBase(BWAPI::Unit base)
:mp_resourceDepot(base)
, infolevel(0)
, scanRange(350)
, mp_miningAlgo(NULL)
, mp_workerSet(NULL)
{
	std::clog << "creating mining base" << std::endl;
	//Get a unit set with all the workers near the base
	mp_workerSet = new std::unordered_map<int, WorkerUnit*>;
	BWAPI::Unitset mp_worker = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle && BWAPI::Filter::IsOwned);
	for (auto w : mp_worker)
	{
		addWorker(w);
	}
	//Get unit set with mineral patches
	mp_minerals = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsMineralField);
	//get unit set with geysers
	mp_geysers = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsResourceContainer && !BWAPI::Filter::IsMineralField);
	//get refinerys near base
	mp_refinerys = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsRefinery);
}

MiningBase::MiningBase(BWAPI::Unit base, int scanrange)
:mp_resourceDepot(base)
, infolevel(0)
, scanRange(scanrange)
, mp_miningAlgo(NULL)
, mp_workerSet(NULL)
{
	std::clog << "creating mining base" << std::endl;
	//Get a unit set with all the workers near the base
	mp_workerSet = new std::unordered_map<int,WorkerUnit*>;
	BWAPI::Unitset mp_worker = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle && BWAPI::Filter::IsOwned);
	for (auto w : mp_worker)
	{
		addWorker(w);
	}
	//Get unit set with mineral patches
	mp_minerals = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsMineralField);
	//get unit set with geysers
	mp_geysers = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsResourceContainer && !BWAPI::Filter::IsMineralField);
	//get refinerys near base
	mp_refinerys = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsRefinery);
}


MiningBase::~MiningBase()
{
	//Base is destroyed send all workers to nearest base
	for (auto i : *mp_workerSet)
	{
		delete i.second;
	}
	delete mp_workerSet;
}

void MiningBase::addWorker(BWAPI::Unit worker)
{
	std::clog << "Adding worker with ID: " << worker->getID() << "To base with ID: " << mp_resourceDepot << std::endl;
	WorkerUnit *temp = new WorkerUnit(worker);
	mp_workers.emplace(worker);
	auto i = mp_workerSet->emplace(worker->getID(),temp);
	//if second value is true it already exists in the list so delete the pointer
	if (!i.second)
	{
		std::cerr << "Adding worker failed, worker ID is: " << worker->getID() << std::endl;
		delete temp;
		return;
	}
}

void MiningBase::remWorker(BWAPI::Unit worker)
{
	//Delete the worker from both the lists
	mp_workers.erase(worker);
	//Delete the workerunit from the set
	delete mp_workerSet->at(worker->getID());
	mp_workerSet->erase(worker->getID());
}

void MiningBase::addWorkerSet(BWAPI::Unitset workerset)
{
	//must implement Workerunit
	mp_workers.insert(workerset.begin(), workerset.end());
}

void MiningBase::remWorkerSet(BWAPI::Unitset workerset)
{
	for (auto worker : workerset)
	{
		mp_workers.erase(worker);
	}
}

BWAPI::Unitset MiningBase::getWorkerList()
{
	return mp_workers;
}

void MiningBase::setINFOlevel(int level)
{
	infolevel = level;
}

void MiningBase::drawInfo()
{
	//draw scanrange circle
	BWAPI::Broodwar->drawCircleMap(mp_resourceDepot->getPosition(), scanRange, BWAPI::Colors::Purple, false);

	//draw a rectangle around every worker and mineral patch
	for (auto w : mp_workers)
	{
		BWAPI::Broodwar->drawBoxMap(w->getLeft(), w->getTop(), w->getRight(), w->getBottom(), BWAPI::Colors::Red);
	}

	for (auto w : mp_minerals)
	{
		BWAPI::Broodwar->drawBoxMap(w->getLeft(), w->getTop(), w->getRight(), w->getBottom(), BWAPI::Colors::Blue);
	}
}

void MiningBase::setQueue()
{
	if (mp_miningAlgo = NULL)
		mp_miningAlgo = new Queue;
	else
	{
		if (!mp_miningAlgo->isQueue())
		{
			delete mp_miningAlgo;
			mp_miningAlgo = new Queue;
		}
	}
}

void MiningBase::setMineralLock()
{
	if (mp_miningAlgo == NULL)
		mp_miningAlgo = new MineralLock(this);
	else
	{
		if (!mp_miningAlgo->isMineralLock())
		{
			delete mp_miningAlgo;
			mp_miningAlgo = new MineralLock(this);
		}
	}
}

void MiningBase::checkWorkers()
{
	//update state of every worker
	for (auto i : *mp_workerSet)
		i.second->Update();

	//Run mining algo
	if (mp_miningAlgo != NULL)
	{
		mp_miningAlgo->execute();
	}
}

BWAPI::Unitset MiningBase::getResourceContainers()
{
	return mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsResourceContainer);
}

std::unordered_map<int, WorkerUnit*> * MiningBase::getWorkerUnitList()
{
	return mp_workerSet;
}