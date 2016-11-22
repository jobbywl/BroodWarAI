#pragma once
#include <iostream>

class Schedulable
{
public:
	Schedulable();
	virtual ~Schedulable();

	bool operator>(const Schedulable&);
	bool operator<(const Schedulable&);
	bool operator==(const Schedulable&);

	friend std::ostream& operator<<(std::ostream& out, const Schedulable &x)
	{
		out << x.priority;
		return out;
	}

	void setPriority(int);

	virtual void execute() = 0;
private:
	int priority;
};

