#include "EntityManager.h"

#include "Debug.h"
#include "Entity.h"
#include "PositionComponent.h"

#define ENTITY_ARRAY_START_SIZE 64
#define ENTITY_ARRAY_INCREASE 64



EntityManager::EntityManager() {
	initializeEntityArray();
}

EntityManager::~EntityManager() {
	for (int i = 0; i < entityCount; i++) {
		delete entities[i];
	}
	delete[] entities;
}

void EntityManager::registerEntity(Entity* entity) {
	if (entityCount == entityCapacity) {
		growEntityArray();
		if (entityCount == entityCapacity) { // Grow failled
			return;
		}
	}
	entity->id = entityCount;
	entities[entity->id] = entity;
	entityCount++;
}

void EntityManager::unregisterEntity(Entity* entity) {
	int i = 0;
	for (i = entity->id + 1; i < entityCount; i++) {
		entities[i]->id--; // Offset ids to acount for shift
		entities[i - 1] = entities[i];
	}
	entities[i - 1] = nullptr;
	entityCount--;
}

void EntityManager::initializeEntityArray() {
	entityCapacity = ENTITY_ARRAY_START_SIZE;
	entities = new Entity*[entityCapacity];
}

void EntityManager::growEntityArray() {
	entityCapacity += ENTITY_ARRAY_INCREASE;
	Entity** nEntities = new Entity*[entityCapacity];
	if (nEntities == nullptr) {
		Debug::logError("EntityManager", "Could not allocate memory required for additionnal entities");
		return;
	}
	for (int i = 0; i < entityCount; i++) {
		nEntities[i] = entities[i];
	}
	delete[] entities;
	entities = nEntities;
}
