#include "Transform.h"

#include "Debug.h"

#define CHILD_ARRAY_START_SIZE 8
#define CHILD_ARRAY_INCREASE 4



Transform::Transform(Entity* entity) : Component(entity) {

}

Transform::~Transform() {
	if (parent != nullptr) {
		parent->removeChild(childIndex);
	}
	unsigned int deletedCount = 0;
	for (unsigned int i = 0; i < childrenCapacity; i++) {
		if (deletedCount == childrenCount) break;
		if (children[i] == nullptr) continue;
		delete children[i]->entity;
		deletedCount++;
	}
	delete[] children;
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
	if (childrenCount == 0) return;
	unsigned int notified = 0;
	for (unsigned int i = 0; i < childrenCapacity; i++) {
		if (notified == childrenCount) break;
		if (children[i] == nullptr) continue;
		children[i]->localToWorldMatrixExpired = true;
		children[i]->worldToLocalMatrixExpired = true;
		children[i]->notifyChildrenLocalToWorldChange();
		notified++;
	}
}

void Transform::addChild(Transform* child) {
	if (children == nullptr) {
		initChildArray();
	}
	if (childrenCount == childrenCapacity) {
		growChildArray();
		if (childrenCount == childrenCapacity) { // Grow failled
			return;
		}
	}
	unsigned int freeIndex;
	for (freeIndex = 0; freeIndex < childrenCapacity; freeIndex++) {
		if (children[freeIndex] == nullptr) break;
	}
	children[freeIndex] = child;
	child->childIndex = freeIndex;
	childrenCount++;
}

void Transform::removeChild(unsigned int index) {
	children[index] = nullptr;
}

void Transform::initChildArray() {
	childrenCapacity = CHILD_ARRAY_START_SIZE;
	children = new Transform*[childrenCapacity];
	for (int i = 0; i < childrenCapacity; i++) {
		children[i] = nullptr;
	}
}

void Transform::growChildArray() {
	childrenCapacity += CHILD_ARRAY_INCREASE;
	Transform** nChildren = new Transform*[childrenCapacity];
	if (nChildren == nullptr) {
		Debug::logError("Transform", "Could not allocate more memory for additionnal children!");
		return;
	}
	int i;
	for (i = 0; i < childrenCount; i++) {
		nChildren[i] = children[i];
	}
	for (; i < childrenCapacity; i++) {
		nChildren[i] = nullptr;
	}
	delete[] children;
	children = nChildren;
}
