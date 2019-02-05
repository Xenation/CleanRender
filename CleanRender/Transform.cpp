#include "Transform.h"

#include "Debug.h"

#define CHILD_ARRAY_START_SIZE 4
#define CHILD_ARRAY_INCREASE 4



Transform::Transform(Entity* entity)
	: Component(entity), children(SimpleList<Transform*>(CHILD_ARRAY_START_SIZE, CHILD_ARRAY_INCREASE)) {

}

Transform::~Transform() {
	if (parent != nullptr) {
		parent->removeChild(childIndex);
	}
	for (unsigned int i = 0; i < children.count; i++) {
		delete children[i]->entity;
	}
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

void Transform::setParent(Transform* parent) {
	if (this->parent == parent) return;
	if (this->parent != nullptr) {
		this->parent->removeChild(childIndex);
	}
	this->parent = parent;
	if (this->parent != nullptr) {
		this->parent->addChild(this);
	}
	localToWorldMatrixExpired = true;
	worldToLocalMatrixExpired = true;

}

void Transform::invalidateMatrices() {
	modelMatrixExpired = true;
	localToWorldMatrixExpired = true;
	worldToLocalMatrixExpired = true;
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
	localToWorldMatrix = (parent == nullptr) ? modelMatrix : parent->getLocalToWorldMatrix() * modelMatrix;
	localToWorldMatrixExpired = false;
	worldToLocalMatrixExpired = true;
	notifyChildrenLocalToWorldChange();
}

void Transform::recalculateWTLMatrix() {
	if (localToWorldMatrixExpired) {
		recalculateLTWMatrix();
	}
	worldToLocalMatrix = localToWorldMatrix.inverseAffine();
	worldToLocalMatrixExpired = false;
}

void Transform::notifyChildrenLocalToWorldChange() {
	if (children.count == 0) return;
	for (unsigned int i = 0; i < children.count; i++) {
		children[i]->localToWorldMatrixExpired = true;
		children[i]->worldToLocalMatrixExpired = true;
		children[i]->notifyChildrenLocalToWorldChange();
	}
}

void Transform::addChild(Transform* child) {
	child->childIndex = children.add(child);
}

void Transform::removeChild(unsigned int index) {
	children.remove(index);
	for (unsigned int i = index; i < children.count; i++) {
		children[i]->childIndex--;
	}
}
