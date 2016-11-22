#include "Scheduler.h"


Scheduler::Scheduler()
:mp_Queue(0)
{
	mp_Queue = new std::priority_queue<Schedulable*>;
}


Scheduler::~Scheduler()
{
	delete mp_Queue;
	mp_Queue = NULL;
}

void Scheduler::addTask(Schedulable* task)
{
	mp_Queue->push(task);
}

void Scheduler::run()
{
	while (mp_Queue->size() > 0)
	{
		//Execute and delete
		Schedulable *temp;
		temp = mp_Queue->top();
		temp->execute();
		delete temp;
		mp_Queue->pop();
	}
}