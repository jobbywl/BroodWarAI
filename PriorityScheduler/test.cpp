#include <iostream>
#include "Scheduler.h"
#include "ExampleTask.h"


#include <vld.h>
#pragma comment(lib,"vld.lib")

int main()
{
	Scheduler sched;
	sched.addTask(new ExampleTask(10));
	sched.addTask(new ExampleTask(11));
	sched.addTask(new ExampleTask(9));

	sched.run();
	return 0;
}