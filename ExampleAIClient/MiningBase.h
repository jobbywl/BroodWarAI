#pragma once
#include "BWAPI.h"
#include "MiningAlgo.h"
#include <set>
#include "WorkerUnit.h"

namespace Worker
{

	class MiningBase
	{
	public:
		MiningBase(BWAPI::Unit, int =350);
		virtual ~MiningBase();

		void addWorker(BWAPI::Unit);
		void remWorker(BWAPI::Unit);

		void addWorkerSet(BWAPI::Unitset);
		void remWorkerSet(BWAPI::Unitset);

		BWAPI::Unitset getWorkerList();
		std::unordered_map<int, WorkerUnit*> * getWorkerUnitList();

		void checkWorkers();

		bool isSaturated();
		void setSaturated(bool);

		//Set algo's
		void setQueue();
		void setMineralLock();

		//Debug only
		void setINFOlevel(int);
		void drawInfo();

		BWAPI::Unitset getResourceContainers();

		BWAPI::Unit getResourceDepot();

	private:
		BWAPI::Unit mp_resourceDepot;
		std::unordered_map<int,WorkerUnit*> *mp_workerSet;	//This is used for worker micro
		BWAPI::Unitset mp_workers;			//This is a list used for emergency evacuations
		BWAPI::Unitset mp_minerals;
		BWAPI::Unitset mp_geysers;
		BWAPI::Unitset mp_refinerys;
		MiningAlgo *mp_miningAlgo;

		int scanRange;
		bool saturated;

		//Debug stuff
		int infolevel;
	};
}