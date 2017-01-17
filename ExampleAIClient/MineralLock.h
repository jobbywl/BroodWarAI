#pragma once
#include <map>
#include "BWAPI.h"
#include "MiningAlgo.h"
#include "WorkerUnit.h"
#include "MiningBase.h"

namespace Worker
{

	class MineralLock :
		public MiningAlgo
	{
	public:
		MineralLock(MiningBase *base);
		virtual ~MineralLock();

		void execute();

		bool isMineralLock();
	private:
		std::multimap<int,BWAPI::Unit> *mp_minerals;
		MiningBase *mp_base;

		void addWorker(WorkerUnit*);
	};
}