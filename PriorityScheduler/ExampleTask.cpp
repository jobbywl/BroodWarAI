#include "ExampleTask.h"
#include <iostream>


ExampleTask::ExampleTask(int priority)
{
	this->setPriority(priority);
}


ExampleTask::~ExampleTask()
{
}


void ExampleTask::execute()
{
	Schedulable *temp = (Schedulable*)this;
	std::cout << "Task with prio: " << *temp << std::endl;
}