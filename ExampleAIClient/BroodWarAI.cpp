#include "BroodWarAI.h"
#include <iostream>


BroodWarAI::BroodWarAI()
{
	mp_Worker = new Worker::WorkerManager;
	std::clog << "Ai created" << std::endl;
}


BroodWarAI::~BroodWarAI()
{
}

void BroodWarAI::onStart()
{
	std::clog << "Ai Started" << std::endl;
	//Add all the units to the first base
	BWAPI::Unitset temp = BWAPI::Broodwar->getAllUnits();

	for (auto all : temp)
	{
		if (all->getType().isResourceDepot())
		{
			//Get a unit set with all the workers near the base
			BWAPI::Unitset workers = all->getUnitsInRadius(12, BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle && BWAPI::Filter::IsOwned);
			//Convert the workers to a list
			std::list<BWAPI::Unit> *list = new std::list<BWAPI::Unit>;
			for (auto w : workers)
				list->push_back(w);

			mp_Worker->addBase((BWAPI::Unit)all, list);
		}
	}

}//

void BroodWarAI::onEnd(bool isWinner)
{}//

void BroodWarAI::onFrame()
{
	/*
	BWAPI::Unitset units = BWAPI::Broodwar->getAllUnits();
	//Convert the workers to a list
	for (auto u : units)
	{
		//Add all units to appropiate manager
		if (u->getType().isWorker() && u->isIdle())
		{
			BWAPI::Unit base = u->getClosestUnit(BWAPI::Filter::IsResourceDepot);
			mp_Worker->addWorkerToBase(base, (BWAPI::Unit)u);
		}
	}
	*/
	//Micro all the mineral gatherers
	mp_Worker->checkWorkers();
}//

void BroodWarAI::onSendText(std::string text)
{}//

void BroodWarAI::onReceiveText(BWAPI::Player player, std::string text)
{}//

void BroodWarAI::onPlayerLeft(BWAPI::Player player)
{}//

void BroodWarAI::onNukeDetect(BWAPI::Position target)
{}//

void BroodWarAI::onUnitDiscover(BWAPI::Unit unit)
{}

void BroodWarAI::onUnitEvade(BWAPI::Unit unit)
{}

void BroodWarAI::onUnitShow(BWAPI::Unit unit)
{}//

void BroodWarAI::onUnitHide(BWAPI::Unit unit)
{}//

void BroodWarAI::onUnitCreate(BWAPI::Unit unit)
{}//

void BroodWarAI::onUnitDestroy(BWAPI::Unit unit)
{}//

void BroodWarAI::onUnitMorph(BWAPI::Unit unit)
{}//

void BroodWarAI::onUnitRenegade(BWAPI::Unit unit)
{}//

void BroodWarAI::onSaveGame(std::string gameName)
{}//

void BroodWarAI::onUnitComplete(BWAPI::Unit unit)
{
	//Add all units to appropiate manager
	if (unit->getType().isWorker() && unit->isIdle())
	{
		BWAPI::Unit base = unit->getClosestUnit(BWAPI::Filter::IsResourceDepot);
		mp_Worker->addWorkerToBase(base, (BWAPI::Unit)unit);
	}
	else if (unit->getType().isResourceDepot())
	{
		mp_Worker->addBase(unit);
	}
}//
