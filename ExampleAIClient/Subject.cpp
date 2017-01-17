#include "Subject.h"
#include "Observer.h"

Subject::Subject()
{
}


Subject::~Subject()
{
	if (!mp_observerCollection.empty())
		mp_observerCollection.clear();
}

void Subject::AddObserver(Observer* s)
{
	mp_observerCollection.push_back(s);
}

void Subject::DelObserver(Observer* s)
{
	mp_observerCollection.remove(s);
}

void Subject::Notify()
{
	for (std::list<Observer*>::iterator i = mp_observerCollection.begin(); i != mp_observerCollection.end(); ++i)
		(*i)->Update(this);
}