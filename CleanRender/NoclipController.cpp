#include "NoclipController.h"

#include "Entity.h"
#include "Time.h"
#include "Input.h"
#include "Transform.h"



NoclipController::NoclipController(Entity* entity) : Component(entity) {

}

NoclipController::~NoclipController() {

}

void NoclipController::onUpdate() {
	float dt = Time::deltaTime;
	float currentSpeed = speed;
	if (Input::lShiftPressed) {
		currentSpeed *= 4;
	}
	if (Input::wPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->forward() * currentSpeed * dt);
	}
	if (Input::sPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->backward() * currentSpeed * dt);
	}
	if (Input::dPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->right() * currentSpeed * dt);
	}
	if (Input::aPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->left() * currentSpeed * dt);
	}
	if (Input::qPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->down() * currentSpeed * dt);
	}
	if (Input::ePressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->up() * currentSpeed * dt);
	}
	if (Input::mouseRightPressed) {
		eulerRot.y += Input::mouseDelta.x * lookSensivity * dt;
		eulerRot.x += Input::mouseDelta.y * lookSensivity * dt;
		Input::mouseDelta = Vec2f::zero; // TODO remove when input system better
		entity->transform->setRotation(Quaternion::euler(eulerRot));
	}
}
