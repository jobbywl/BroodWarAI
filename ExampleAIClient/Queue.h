#pragma once
#include "MiningAlgo.h"

namespace Worker
{
	class Queue :
		public MiningAlgo
	{
	public:
		Queue();
		virtual ~Queue();
		void execute();

		bool isQueue();
	};
}