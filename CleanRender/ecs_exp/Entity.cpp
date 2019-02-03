#include "Entity.h"

#include "Engine.h"
#include "EntityManager.h"



Entity::Entity()
	: componentFlags((char*) this + sizeof(Entity)) { // Pointer initialization here because entering the constructor clear it
	clearComponentFlags();
	Engine::entityManager->registerEntity(this);
}

Entity::Entity(const Entity& entity) {

}

Entity::~Entity() {
	Engine::entityManager->unregisterEntity(this);
}

void Entity::addComponent(BaseComponent component) {

}

void Entity::removeComponent(unsigned int flagIndex) {

}

bool Entity::hasComponents(char* flags) {
	for (int i = 0; i < BaseComponent::typesBitFieldSize; i++) {
		if (componentFlags[i] & flags[i] != flags[i]) {
			return false;
		}
	}
	return true;
}

void Entity::clearComponentFlags() {
	for (int i = 0; i < BaseComponent::typesBitFieldSize; i++) {
		componentFlags[i] = 0;
	}
}
