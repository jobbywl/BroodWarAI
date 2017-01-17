#ifndef SUBJECT_H_
#define SUBJECT_H_

#include <list>

class Observer;

class Subject
{


public:
	Subject();
	virtual ~Subject();
	virtual void AddObserver(Observer* s);
	virtual void DelObserver(Observer* s);

protected:
	//This must be called on when you want to notify the observers
	virtual void Notify();

private:
	std::list<Observer*> mp_observerCollection;
};


#endif /*SUBJECT_H_*/