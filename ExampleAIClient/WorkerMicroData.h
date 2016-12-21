#pragma once
#include <queue>
//Here the necessary data structures are defined for the different micro types
//This is done to make everything more readable
namespace Worker{

	struct MineralLockData
	{
		
	};

	struct QueueData
	{
		//Map an unit pointer to in index with which we can acces the necessary data
		std::map<BWAPI::Unit, int> mineralMap;

		//queue must be a pointer else we cant elements 
		std::vector<BWAPI::Unit> mineralVector;
		std::vector<double> mineralTimer;
		std::map<BWAPI::Unit,bool> previousWorkerState;

		const double gatherTime = 90;
	};

	struct CoopPathfindingData
	{

	};
}