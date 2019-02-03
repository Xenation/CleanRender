#pragma once
class Entity;
class BaseSystem {
public:
	typedef unsigned int Id;
	
	char* componentFilter;
	Id id;

	virtual void update() = 0;
	virtual void onMatchingEntityCreated() = 0;
protected:
	Entity** entities;

	void filter(unsigned int compId);
};

