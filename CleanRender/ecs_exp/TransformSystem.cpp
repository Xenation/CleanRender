#include "TransformSystem.h"

#include "PositionComponent.h"



TransformSystem::TransformSystem() {
	filter(PositionComponent::typeId);
}

TransformSystem::~TransformSystem() {}


void TransformSystem::update() {

}
