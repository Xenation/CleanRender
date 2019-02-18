#include "Transform.h"

#include "Debug.h"
#include "Entity.h"

#define CHILD_ARRAY_START_SIZE 4
#define CHILD_ARRAY_INCREASE 4



Transform::Transform(Entity* entity)
	: Component(entity, this) {

}

Transform::~Transform() {
	
}


Matrix4x4f Transform::getLocalToWorldMatrix() {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	return localToWorldMatrix;
}

Matrix4x4f Transform::getWorldToLocalMatrix() {
	if (worldToLocalMatrixExpired) {
		recalculateWTLMatrix();
	}
	return worldToLocalMatrix;
}

Vec4f Transform::localToWorld(Vec4f vec) {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	return localToWorldMatrix * vec;
}

Vec3f Transform::localToWorldPos(Vec3f pos) {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	return localToWorldMatrix.multPoint(pos);
}

Vec3f Transform::localToWorldDir(Vec3f dir) {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	return localToWorldMatrix.multDirection(dir);
}

Vec4f Transform::worldToLocal(Vec4f vec) {
	if (worldToLocalMatrixExpired) {
		recalculateWTLMatrix();
	}
	return worldToLocalMatrix * vec;
}

Vec3f Transform::worldToLocalPos(Vec3f pos) {
	if (worldToLocalMatrixExpired) {
		recalculateWTLMatrix();
	}
	return worldToLocalMatrix.multPoint(pos);
}

Vec3f Transform::worldToLocalDir(Vec3f dir) {
	if (worldToLocalMatrixExpired) {
		recalculateWTLMatrix();
	}
	return worldToLocalMatrix.multDirection(dir);
}

void Transform::parentChanged() {
	localToWorldMatrixExpired = true;
	worldToLocalMatrixExpired = true;
}

void Transform::invalidateMatrices() {
	modelMatrixExpired = true;
	localToWorldMatrixExpired = true;
	worldToLocalMatrixExpired = true;
	worldPositionExpired = true;
}

void Transform::recalculateModelMatrix() {
	modelMatrix = Matrix4x4f::transformation(position, scale, rotation);
	modelMatrixExpired = false;
	localToWorldMatrixExpired = true;
	worldToLocalMatrixExpired = true;
}

void Transform::recalculateLTWMatrix() {
	if (modelMatrixExpired) {
		recalculateModelMatrix();
	}
	localToWorldMatrix = modelMatrix;
	if (entity->getParent() != nullptr) {
		Entity* currentParent = entity->getParent();
		while (currentParent != nullptr) {
			if (currentParent->transform != nullptr) {
				localToWorldMatrix = currentParent->transform->getLocalToWorldMatrix() * modelMatrix;
				break;
			}
			currentParent = currentParent->getParent();
		}
	}
	entity->ltwChangeNotifyChildren();
	localToWorldMatrixExpired = false;
	worldToLocalMatrixExpired = true;
}

void Transform::recalculateWTLMatrix() {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	worldToLocalMatrix = localToWorldMatrix.inverseAffine();
	worldToLocalMatrixExpired = false;
}

void Transform::recalculateWorldPosition() {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	worldPosition = localToWorldMatrix.multPoint(position);
}
