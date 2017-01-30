#pragma once
#include "MiningAlgo.h"
#include "MiningBase.h"

namespace Worker
{
	class Queue :
		public MiningAlgo
	{
	public:
		Queue(MiningBase * base);
		virtual ~Queue();
		void execute();

		bool isQueue();

	private:
		std::multimap<int, BWAPI::Unit> *mp_minerals;
		//First int is ID second is timer
		std::unordered_map<int, int> *mp_mineralTimer;

		MiningBase *mp_base;
	};
}