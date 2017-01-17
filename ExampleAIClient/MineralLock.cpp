#include "MineralLock.h"
#include "FrameCounter.h"

using namespace Worker;

MineralLock::MineralLock(MiningBase *base)
:mp_minerals(NULL)
, mp_base(base)
{
	mp_minerals = new std::multimap<int,BWAPI::Unit>;

	for (auto m : mp_base->getResourceContainers())
	{
		if (m->getType().isMineralField())
			mp_minerals->emplace(mp_base->getResourceDepot()->getDistance(m),m);
	}
	for (auto m : mp_base->getResourceContainers())
	{
		if (m->getType().isMineralField())
			mp_minerals->emplace(mp_base->getResourceDepot()->getDistance(m)*2, m);
	}
}

MineralLock::~MineralLock()
{
	delete mp_minerals;
}


void MineralLock::execute()
{
	for (auto worker : *(mp_base->getWorkerUnitList()))
	{
		if (worker.second->isMineralGatherer() && worker.second->getUnitInterface()->isIdle())
			worker.second->gather();
		else if ((!worker.second->isGasGatherer()) && (!worker.second->isBuilder()) && worker.second->isIdle())
			addWorker(worker.second);
		else if (worker.second->isMineralGatherer() && !worker.second->getUnitInterface()->isCarryingMinerals())
		{
			BWAPI::Unit temp = worker.second->getUnitInterface()->getOrderTarget();
			int id1;
			if (temp != NULL)
			{
				id1 = temp->getID();
				int id2 = worker.second->getResource()->getID();
				if (id1 != id2)
					worker.second->gather();
			}
			 
		}
			
	}
	
}

bool MineralLock::isMineralLock()
{
	return true;
}

void MineralLock::addWorker(WorkerUnit* worker)
{
	if (mp_minerals->size() > 0)
	{
		BWAPI::Unit closest = mp_minerals->begin()->second;
		mp_minerals->erase(mp_minerals->begin());


		BWAPI::Unit temp = worker->setResource(closest);
	}
	else
	{
		mp_base->setSaturated(true);
	}
}
