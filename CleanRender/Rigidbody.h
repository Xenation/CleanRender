#pragma once

#include "bullet/btBulletDynamicsCommon.h"
#include "Component.h"

class Transform;
class Entity;

class SynchronizedTransform : public btMotionState {
public:
	Transform* transform;

	SynchronizedTransform(Transform* transform);
	~SynchronizedTransform();

	void getWorldTransform(btTransform& worldTransform) const override;
	void setWorldTransform(const btTransform& worldTransform) override;
};

class Rigidbody : public Component {
public:
	Rigidbody(Entity* entity);
	~Rigidbody();

	void Initialize(btCollisionShape* shape, float mass);
	inline btRigidBody* getBulletBody() { return body; }

private:
	float mass = 0.0f;
	SynchronizedTransform syncTransform;
	btRigidBody* body;
};

