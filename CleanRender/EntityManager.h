#pragma once
#include "HollowSet.h"
class Entity;
class EntityManager {
public:
	EntityManager();
	~EntityManager();
	
	void registerEntity(Entity* entity);
	void unregisterEntity(Entity* entity);

	void updateEntities();

private:
	HollowSet<Entity*> entities;
};

