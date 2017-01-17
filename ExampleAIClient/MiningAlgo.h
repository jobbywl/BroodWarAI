#pragma once

namespace Worker
{
	class MiningAlgo
	{
	public:
		virtual void execute() = 0;
		virtual bool isQueue(){ return false; }
		virtual bool isMineralLock(){ return false; }
	};
}