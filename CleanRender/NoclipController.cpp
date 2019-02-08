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
	if (transform == nullptr) return;
	float dt = Time::deltaTime;
	float currentSpeed = speed;
	if (Input::lShiftPressed) {
		currentSpeed *= 4;
	}
	if (Input::wPressed) {
		transform->translate(transform->forward() * currentSpeed * dt);
	}
	if (Input::sPressed) {
		transform->translate(transform->backward() * currentSpeed * dt);
	}
	if (Input::dPressed) {
		transform->translate(transform->right() * currentSpeed * dt);
	}
	if (Input::aPressed) {
		transform->translate(transform->left() * currentSpeed * dt);
	}
	if (Input::qPressed) {
		transform->translate(transform->down() * currentSpeed * dt);
	}
	if (Input::ePressed) {
		transform->translate(transform->up() * currentSpeed * dt);
	}
	if (Input::mouseRightPressed) {
		eulerRot.y += Input::mouseDelta.x * lookSensivity * dt;
		eulerRot.x += Input::mouseDelta.y * lookSensivity * dt;
		Input::mouseDelta = Vec2f::zero; // TODO remove when input system better
		entity->transform->setRotation(Quaternion::euler(eulerRot));
	}
}
