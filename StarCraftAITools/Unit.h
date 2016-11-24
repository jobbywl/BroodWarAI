#pragma once
#include <list>
#include <string>

enum unitSize
{
	S,
	M,
	L
};

struct unitAttack
{
	bool explosive;
	bool concussive;
	bool splash;
};

class Unit
{
public:
	Unit(std::list<std::string>*);
	virtual ~Unit();

	int add();
	int add(int);
private:
	//stats
	std::string name;
	unitSize size;
	int supply;
	int minerals;
	int gas;
	int armor;
	int hp;
	int groundAttack;
	unitAttack groundAttackType;
	int airAttack;
	unitAttack airAttackType;
	int attackSpeed;
	int range;
	int attackMod;
	int sight;
	int buildTime;
	std::string notes;

	//current
	int Amount;
};

