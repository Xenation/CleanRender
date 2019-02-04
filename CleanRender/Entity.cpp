#include "Entity.h"

#include "Debug.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Transform.h"

#define COMPONENTS_ARRAY_START_SIZE 4
#define COMPONENTS_ARRAY_INCREASE 1



Entity::Entity()
	: components(HollowSet<Component*>(COMPONENTS_ARRAY_START_SIZE, COMPONENTS_ARRAY_INCREASE)) {
	transform = addComponent(new Transform(this));
	Engine::entityManager->registerEntity(this);
}

Entity::~Entity() {
	isDying = true;
	Engine::entityManager->unregisterEntity(this);
	unsigned int deletedCount = 0;
	for (unsigned int i = 0; i < components.capacity; i++) {
		if (deletedCount == components.count) break;
		if (components[i] == nullptr) continue;
		if (components[i]->isEnabled()) {
			components[i]->onDisable();
		}
		components[i]->onDestroy();
		delete components[i];
		deletedCount++;
	}
}

void Entity::updateComponents() {
	unsigned int updated = 0;
	for (unsigned int i = 0; i < components.capacity; i++) {
		if (updated == components.count) break;
		if (components[i] == nullptr) continue;
		components[i]->onUpdate();
		updated++;
	}
}
