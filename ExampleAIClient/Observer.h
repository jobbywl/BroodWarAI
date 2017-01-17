#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <list>
#include <vector>
class Subject;

class Observer
{

public:
	Observer();
	virtual ~Observer();
	virtual void AddSubject(Subject* s);
	virtual void DelSubject(Subject* s);

	//Gets called one every notify, Must be implementented by observer. Also Contains a pointer to the object that called it
	virtual void Update(Subject *s) = 0;

private:
	std::list<Subject*> mp_subjectCollection;
};



#endif /*OBSERVER_H_*/