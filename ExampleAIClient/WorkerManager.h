#pragma once
//This manager makes sure there are enough workers and they are micro'd correctly

//http://www.teamliquid.net/forum/brood-war/484849-improving-mineral-gathering-rate-in-brood-war
/*
Instead of letting the worker unit return to the last mineral field it gathered from, the queue system calculates for each mineral field how long it will take before the worker unit will return again (with minerals of course).

This calculation is based on:

1. how long it takes to travel to the mineral field
2. How long the other worker units assigned to the mineral field will occupy the field
3. How long it takes to travel back to the resource depot

For the second calculation ,the queue system maintains a queue for each mineral field.
Given the travel time, the queue system can determine for how long the mineral field will still be occupied when the worker unit arrives.

Not used by any human that I know of.
Christensen et al. originally developed this system and experimented with it, but it hasn't been put in any BWAPI bot that I know of.

*/

/*
Another trick that humans use (besides mineral lock) to increase the mineral gathering rate.
When a worker unit approaches a mineral field that it wants to gather from, it will start to brake when it comes close to the mineral field.

This behaviour can be prevented by telling the worker unit to move to a location beyond the mineral field and switching back to the mineral field when the SCV nearly touches the mineral field.
This way the SCV won't slowly break, but go full speed all the way to the mineral field.

Two examples are given in the picture above. In the first option, the worker unit want to get to the leftmost mineral field (indicated by a red circle with a square inside).
To arrive there faster, it gets an order to mine the mineral field beyond its original target (indicated by a green circle with a triangle inside). When close enough, it switches back to the original intended mineral field.

In the second option, the worker unit wants to get to the uppermost mineral field (indicated by a red circle with a filled circle inside). Instead of using another mineral field, it uses a move command that goes beyond the mineral field (indicated by a filled green circle above the mineral field).
When close enough, it switches back to the original indented mineral field.

When using a move command, the collision detection for the worker unit is turned on again. So co-operative pathfinding is used to avoid collisions (details of this will be in my master thesis).

Used sometimes by humans in the early stages of the game to gain a mineral lead on their opponent.
*/

#include <BWAPI.h>

namespace Worker
{

	struct MiningBase
	{
		const BWAPI::Unit depot;	//Command center, hatchery, nexus
		std::list<BWAPI::Unit> *workers;
		std::map<BWAPI::Unit, BWAPI::Unit> minerals;
	};

	class WorkerManager
	{
	public:
		WorkerManager();
		~WorkerManager();

		void addBase(BWAPI::Unit base);
		void addBase(BWAPI::Unit base, std::list<BWAPI::Unit> *worker);
		void addWorkerToBase(BWAPI::Unit base, BWAPI::Unit worker);
		void addWorkerToBase(BWAPI::Unit base, std::list<BWAPI::Unit> *worker);

		void setMineralLock(bool);
		void setqueueSystem(bool);
		void setcoopPathfinding(bool);

		bool getMineralLock();
		bool getqueueSystem();
		bool getcoopPathfinding();


		void checkWorkers();

	private:
		std::list<MiningBase*> *mp_basesList;

		bool m_mineralLock;
		bool m_queueSystem;
		bool m_coopPathfinding;


		void mineralLock();
		void queueSystem();
		void coopPathfinding();
	};

}