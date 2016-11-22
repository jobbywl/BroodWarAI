#pragma once
#include "Race.h"
#include <list>
#include <map>
#include "Unit.h"

enum TerranUnits
{
	BattleCruiser,
	Firebat,
	Ghost,
	Goliath,
	Marine,
	Medic,
	ScienceVessel,
	DropShip,
	SCV,
	SiegeTank,
	SpiderMine,
	Valkyrie,
	Vulture,
	Wraith
};

enum TerranBuildings
{
	CommandCenter,
	ComsatStation,
	NuclearSilo,
	SupplyDepot,
	Refinery,
	Barracks,
	EngineeringBay,
	Bunker,
	Academy,
	MissileTurret,
	Factory,
	MachineShop,
	Starport,
	ControlTower,
	Armory,
	ScienceFacility,
	PhysicsLab,
	CovertOps
};

enum TerranUpgrades
{
	InfantryArmor,
	InfantryWeapons,
	ShipPlating,
	ShipWeapons,
	VehiclePlating,
	VehicleWeapons
};

class Terran :
	public Race
{
public:
	Terran();
	~Terran();

	void AddUnit(TerranUnits);
	void AddBuilding(TerranBuildings);
	void AddUpgrade(TerranUpgrades);

private:
	std::map<std::string,Unit*> *mp_Units;
	std::map<std::string, Unit*> *mp_Buildings;
	std::map<std::string, Unit*> *mp_Upgrades;
};

