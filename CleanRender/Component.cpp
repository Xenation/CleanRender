#include "Component.h"

#include "Entity.h"



Component::Component(Entity* entity) {
	this->entity = entity;
}

Component::~Component() {
	if (entity->isDying) return;
	entity->removeComponent(this);
}
