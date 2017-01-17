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
		std::unordered_map<BWAPI::Unit, int> *mp_minerals;
		MiningBase *mp_base;

		void addWorker(WorkerUnit*);
	};
}