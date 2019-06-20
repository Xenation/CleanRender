#include "Rigidbody.h"

#include "Transform.h"
#include "Entity.h"
#include "Engine.h"
#include "PhysicsWorld.h"



SynchronizedTransform::SynchronizedTransform(Transform* transform) : transform(transform) {
	
}

SynchronizedTransform::~SynchronizedTransform() {

}


void SynchronizedTransform::getWorldTransform(btTransform& worldTransform) const {
	Vec3f pos = transform->getWorldPosition();
	Quaternion rot = transform->getWorldRotation();
	worldTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
	worldTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
}

void SynchronizedTransform::setWorldTransform(const btTransform& worldTransform) {
	btVector3 pos = worldTransform.getOrigin();
	btQuaternion rot = worldTransform.getRotation();
	transform->setWorldPosition(Vec3f(pos.x(), pos.y(), pos.z()));
	transform->setWorldRotation(Quaternion(rot.x(), rot.y(), rot.z(), rot.w()));
}



Rigidbody::Rigidbody(Entity* entity) : Component(entity), syncTransform(entity->transform) {
	
}

Rigidbody::~Rigidbody() {
	if (body != nullptr) {
		Engine::physicsWorld->UnregisterRigidbody(this);
		delete body;
	}
}


void Rigidbody::Initialize(btCollisionShape* shape, float mass) {
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, &syncTransform, shape);
	body = new btRigidBody(rbInfo);
	Engine::physicsWorld->RegisterRigibody(this);
}
