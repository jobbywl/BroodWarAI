#pragma once
#include "BWAPI.h"

namespace Worker
{
	class MiningAlgo
	{
	public:
		virtual void execute() = 0;
		virtual bool isQueue(){ return false; }
		virtual bool isMineralLock(){ return false; }

		//Calculate how long it takes a to travel to b using the speed and accel of a.
		virtual int calcTravelTime(BWAPI::Unit a,BWAPI::Unit b);
	};
}