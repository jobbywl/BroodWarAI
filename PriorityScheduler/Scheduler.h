#pragma once
#include <queue>
#include "Schedulable.h"

class Scheduler
{
public:
	Scheduler();
	~Scheduler();

	void addTask(Schedulable*);
	void run();
private:
	std::priority_queue<Schedulable*> *mp_Queue;
};

