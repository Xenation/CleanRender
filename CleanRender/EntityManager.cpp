#include "EntityManager.h"

#include "Debug.h"
#include "Entity.h"

#define ENTITY_ARRAY_START_SIZE 64
#define ENTITY_ARRAY_INCREASE 64



EntityManager::EntityManager() 
	: entities(HollowSet<Entity*>(ENTITY_ARRAY_START_SIZE, ENTITY_ARRAY_INCREASE)) {
}

EntityManager::~EntityManager() {
	for (unsigned int i = 0; i < entities.count; i++) {
		delete entities[i];
	}
}

void EntityManager::registerEntity(Entity* entity) {
	entity->id = entities.add(entity);
}

void EntityManager::unregisterEntity(Entity* entity) {
	entities.remove(entity->id);
}

void EntityManager::updateEntities() {
	unsigned int updated = 0;
	for (unsigned int i = 0; i < entities.capacity; i++) {
		if (updated == entities.count) break;
		if (entities[i] == nullptr) continue;
		entities[i]->updateComponents();
		updated++;
	}
}
