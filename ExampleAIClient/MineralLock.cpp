#include "MineralLock.h"
#include "FrameCounter.h"

using namespace Worker;

MineralLock::MineralLock(MiningBase *base)
:mp_minerals(NULL)
, mp_base(base)
{
	mp_minerals = new std::unordered_map<BWAPI::Unit, int>;

	for (auto m : mp_base->getResourceContainers())
	{
		if (m->getType().isMineralField())
			mp_minerals->emplace(m, 1);
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
		else if ((!worker.second->isGasGatherer()) && (!worker.second->isBuilder()))
			addWorker(worker.second);
	}
	
}

bool MineralLock::isMineralLock()
{
	return true;
}

void MineralLock::addWorker(WorkerUnit* worker)
{
	BWAPI::Unit closest = mp_minerals->begin()->first;
	for (auto m : *mp_minerals)
	{
		if (m.second > 0)
		{
			if (closest->getDistance(worker->getUnitInterface()) > m.first->getDistance(worker->getUnitInterface()))
			{
				closest = m.first;
			}
		}
	}

	if (mp_minerals->at(closest) > 0)
	{
		worker->setResource(closest);
		mp_minerals->at(closest)--;
	}
}
