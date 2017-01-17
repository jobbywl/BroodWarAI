#include "BroodWarAI.h"
#include <iostream>


BroodWarAI::BroodWarAI()
:mp_Worker(NULL)
{

	std::clog << "Ai created" << std::endl;
}


BroodWarAI::~BroodWarAI()
{
}

void BroodWarAI::onStart()
{
	std::clog << "creating worker manager" << std::endl;
	if (mp_Worker == NULL)
		mp_Worker = new Worker::WorkerManager;
	std::clog << "Ai Started" << std::endl;

}//

void BroodWarAI::onEnd(bool isWinner)
{
	if (mp_Worker != NULL)
		delete mp_Worker;
	mp_Worker = NULL;
}//

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
	//First bases then workers
	if (unit->getType().isResourceDepot())
	{
		mp_Worker->addBase(unit);
	}
	else if (unit->getType().isWorker() && unit->isIdle())
	{
		BWAPI::Unit base = unit->getClosestUnit(BWAPI::Filter::IsResourceDepot);
		mp_Worker->addWorkerToBase(base, (BWAPI::Unit)unit);
	}
}//
