#pragma once
#include "BWAPI.h"

namespace Worker
{
	class WorkerUnit 
	{
	public:
		WorkerUnit(BWAPI::Unit);
		~WorkerUnit();

		bool deliveredMinerals();

		BWAPI::Unit setResource(BWAPI::Unit);
		BWAPI::Unit getResource();

		void gather();
		void move(BWAPI::Position);
		bool isMineralGatherer();
		bool isGasGatherer();
		bool isBuilder();
		bool isAlive();
		bool isIdle();

		BWAPI::Unit getUnitInterface();
		
		void Update();

		//debug functions
		void drawInfo();

	private:
		BWAPI::Unit mp_unit;
		//Resource it is locked to
		BWAPI::Unit mp_resource;

		bool wasCarrying;
		bool delivered;
	};

}