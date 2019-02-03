#include "Entity.h"

#include "Debug.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Transform.h"

#define COMPONENTS_ARRAY_START_SIZE 4
#define COMPONENTS_ARRAY_INCREASE 1



Entity::Entity() {
	initComponentsArray();
	transform = addComponent(new Transform(this));
	Engine::entityManager->registerEntity(this);
}

Entity::~Entity() {
	isDying = true;
	Engine::entityManager->unregisterEntity(this);
	unsigned int deletedCount = 0;
	for (unsigned int i = 0; i < componentCapacity; i++) {
		if (deletedCount == componentCount) break;
		if (components[i] == nullptr) continue;
		Component* pt = components[i];
		delete components[i];
		deletedCount++;
	}
	delete[] components;
}


void Entity::initComponentsArray() {
	componentCapacity = COMPONENTS_ARRAY_START_SIZE;
	components = new Component*[componentCapacity];
	for (unsigned int i = 0; i < componentCapacity; i++) {
		components[i] = nullptr;
	}
}

void Entity::growComponentsArray() {
	componentCapacity += COMPONENTS_ARRAY_INCREASE;
	Component** nComponents = new Component*[componentCapacity];
	if (nComponents == nullptr) {
		Debug::logError("Entity", "Could not allocate more memory for an additional component!");
		return;
	}
	unsigned int i;
	for (i = 0; i < componentCount; i++) {
		nComponents[i] = components[i];
	}
	for (; i < componentCapacity; i++) {
		nComponents[i] = nullptr;
	}
	delete[] components;
	components = nComponents;
}

unsigned int Entity::getFreeComponentIndex() {
	unsigned int fIndex;
	for (fIndex = 0; fIndex < componentCapacity; fIndex++) {
		if (components[fIndex] == nullptr) break;
	}
	return fIndex;
}
