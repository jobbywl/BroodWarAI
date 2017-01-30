#include "MiningAlgo.h"

using namespace Worker;

//This function doesnt take acceleration into account. Because the units of these values are unknown
int MiningAlgo::calcTravelTime(BWAPI::Unit a, BWAPI::Unit b)
{
	float maxspeed = a->getType().topSpeed();
	int accel = a->getType().acceleration();
	int halt = a->getType().haltDistance();
	int traveltime = 0;

	int distance = a->getPosition().getApproxDistance(b->getPosition());
	
	traveltime = (float)distance / maxspeed;

	return traveltime;
}