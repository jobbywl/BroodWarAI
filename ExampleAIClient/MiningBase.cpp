#include "MiningBase.h"
#include "Queue.h"
#include "MineralLock.h"

using namespace Worker;

MiningBase::MiningBase(BWAPI::Unit base, int scanrange)
:mp_resourceDepot(base)
, infolevel(0)
, scanRange(scanrange)
, mp_miningAlgo(NULL)
, mp_workerSet(NULL)
, saturated(false)
, trainWorkers(true)
{
	std::clog << "creating mining base" << std::endl;
	//Get a unit set with all the workers near the base
	mp_workerSet = new std::unordered_map<int, WorkerUnit*>;
	BWAPI::Unitset mp_worker = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle && BWAPI::Filter::IsOwned);
	for (auto w : mp_worker)
	{
		addWorker(w);
	}
	//Get unit set with mineral patches, and sort by distance
	BWAPI::Unitset temp = mp_resourceDepot->getUnitsInRadius(scanRange, BWAPI::Filter::IsMineralField);

	while (temp.size() > 0)
	{
		BWAPI::Unit closest = *temp.begin();
		for (auto m : temp)
		{
			if (base->getDistance(closest) > base->getDistance(m))
				closest = m;
		}
		temp.erase(closest);
		mp_minerals.emplace(closest);
	}
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
	auto i = mp_workerSet->emplace(worker->getID(), temp);
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

	for (auto w : mp_minerals)
	{
		BWAPI::Broodwar->drawBoxMap(w->getLeft(), w->getTop(), w->getRight(), w->getBottom(), BWAPI::Colors::Blue);
		BWAPI::Broodwar->drawTextMap(w->getPosition(),"%d",w->getID());
	}
}

void MiningBase::setQueue()
{
	if (mp_miningAlgo == NULL)
		mp_miningAlgo = new Queue(this);
	else
	{
		if (!mp_miningAlgo->isQueue())
		{
			delete mp_miningAlgo;
			mp_miningAlgo = new Queue(this);
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
	if (!saturated && trainWorkers)
	{
		TrainWorkers();
	}
	else if (mp_resourceDepot->isTraining())
	{
		mp_resourceDepot->cancelTrain();
	}
	//update state of every worker
	for (auto i : *mp_workerSet)
	{
		if (i.second->isAlive())
			i.second->Update();		
	}

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

bool MiningBase::isSaturated()
{
	return saturated;
}

void MiningBase::setSaturated(bool val)
{
	saturated = val;
}

BWAPI::Unit MiningBase::getResourceDepot()
{
	return mp_resourceDepot;
}

void MiningBase::startWorkerCreation()
{
	trainWorkers = true;
}

void MiningBase::stopWorkerCreation()
{
	trainWorkers = false;
}

bool MiningBase::isTraingingWorkers()
{
	return trainWorkers;
}

void MiningBase::TrainWorkers()
{
		
		if (BWAPI::Broodwar->canMake(BWAPI::UnitTypes::Terran_SCV) )
		{
			if(mp_resourceDepot->getTrainingQueue().size()==0)
				mp_resourceDepot->train(BWAPI::UnitTypes::Terran_SCV);
		}
		else if (BWAPI::Broodwar->canMake(BWAPI::UnitTypes::Zerg_Drone))
		{
			if (mp_resourceDepot->getTrainingQueue().size() == 0)
				mp_resourceDepot->train(BWAPI::UnitTypes::Zerg_Drone);
		}
		else if (BWAPI::Broodwar->canMake(BWAPI::UnitTypes::Protoss_Probe))
		{
			if (mp_resourceDepot->getTrainingQueue().size() == 0)
				mp_resourceDepot->train(BWAPI::UnitTypes::Protoss_Probe);
		}
}