#pragma once
#include "Schedulable.h"
class ExampleTask :
	public Schedulable
{
public:
	ExampleTask(int priority);
	virtual ~ExampleTask();
	virtual void execute();
};

