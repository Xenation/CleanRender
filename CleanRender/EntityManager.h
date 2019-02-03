#pragma once
class Entity;
class EntityManager {
public:
	EntityManager();
	~EntityManager();
	void registerEntity(Entity* entity);
	void unregisterEntity(Entity* entity);
private:
	Entity** entities = nullptr;
	unsigned int entityCount = 0;
	unsigned int entityCapacity = 0;

	void initializeEntityArray();
	void growEntityArray();
};

