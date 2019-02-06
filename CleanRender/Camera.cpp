#include "Camera.h"

#include "Engine.h"
#include "Pipeline.h"
#include "Entity.h"
#include "Transform.h"
#include "Input.h"
#include "Time.h"



Camera::Camera(Entity* entity) : Component(entity) {
	
}

Camera::~Camera() {

}

void Camera::onEnable() {
	id = Engine::pipeline->registerCamera(this);
}

void Camera::onUpdate() {
	float dt = Time::deltaTime;
	float speed = noclipSpeed;
	if (Input::lShiftPressed) {
		speed *= 4;
	}
	if (Input::wPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->forward() * speed * dt);
	}
	if (Input::sPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->backward() * speed * dt);
	}
	if (Input::dPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->right() * speed * dt);
	}
	if (Input::aPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->left() * speed * dt);
	}
	if (Input::qPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->down() * speed * dt);
	}
	if (Input::ePressed) {
		entity->transform->setPosition(entity->transform->getPosition() + entity->transform->up() * speed * dt);
	}
	if (Input::mouseRightPressed) {
		eulerRot.y += Input::mouseDelta.x * lookSensivity * dt;
		eulerRot.x += Input::mouseDelta.y * lookSensivity * dt;
		Input::mouseDelta = Vec2f::zero; // TODO remove when input system better
		entity->transform->setRotation(Quaternion::euler(eulerRot));
	}
}

void Camera::onDisable() {
	Engine::pipeline->unregisterCamera(id);
}

Matrix4x4f Camera::getProjectionMatrix() {
	if (projectionMatrixExpired) {
		updateProjectionMatrix();
	}
	return projectionMatrix;
}

Matrix4x4f Camera::getViewMatrix() {
	return entity->transform->getWorldToLocalMatrix();
}

void Camera::setFov(float fov) {
	this->fov = fov;
}

void Camera::setNearPlane(float near) {
	nearPlane = near;
}

void Camera::setFarPlane(float far) {
	farPlane = far;
}

void Camera::updateProjectionMatrix() {
	aspectRatio = Engine::pipeline->getAspectRatio();
	projectionMatrix = Matrix4x4f::perspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
	projectionMatrixExpired = false;
}
