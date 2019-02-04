#include "Camera.h"

#include "Engine.h"
#include "Pipeline.h"
#include "Entity.h"
#include "Transform.h"
#include "Input.h"



Camera::Camera(Entity* entity) : Component(entity) {
	
}

Camera::~Camera() {

}

void Camera::onEnable() {
	id = Engine::pipeline->registerCamera(this);
}

void Camera::onUpdate() {
	if (Input::wPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + Vec3f(0, 0, 0.01f));
	}
	if (Input::sPressed) {
		entity->transform->setPosition(entity->transform->getPosition() + Vec3f(0, 0, -0.01f));
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
	projectionMatrix = Matrix4x4f::perspectiveProjection(fov, aspectRatio, nearPlane, farPlane);
	projectionMatrixExpired = false;
	
}
