#include "Observer.h"
#include "Subject.h"

Observer::Observer()
{
}


Observer::~Observer()
{
	//Deattach with all the subjects
	for (std::list<Subject*>::iterator i = mp_subjectCollection.begin(); i != mp_subjectCollection.end(); ++i)
	{
		mp_subjectCollection.erase(i);
		(*i)->DelObserver(this);
	}
	mp_subjectCollection.clear();
}

void Observer::AddSubject(Subject* s)
{
	mp_subjectCollection.push_back(s);
	s->AddObserver(this);
}

void Observer::DelSubject(Subject* s)
{
	mp_subjectCollection.remove(s);
	s->DelObserver(this);
}