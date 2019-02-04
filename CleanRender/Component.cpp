#include "Component.h"

#include "Entity.h"



Component::Component(Entity* entity) {
	this->entity = entity;
}

Component::~Component() {
	if (entity->isDying) return;
	entity->removeComponent(this);
}

void Component::enable() {
	this->enabled = true;
	onEnable();
}

void Component::disable() {
	this->enabled = false;
	onDisable();
}
