#pragma once
#include "XMath.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btCollisionShape;
template<typename T>
class btAlignedObjectArray;
class Rigidbody;

class PhysicsWorld {
public:
	PhysicsWorld();
	~PhysicsWorld();

	void Simulate(float dt);

	void RegisterRigibody(Rigidbody* rigidbody);
	void UnregisterRigidbody(Rigidbody* rigidbody);

private:
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*>* collisionShapes;

	void InitializeDebugWorldContents();
	void DisplayDebugWorldContents();
	void CleanDebugWorldContents();
	void AddDebugRigidBody(btCollisionShape* colShape, Vec3f pos, float mass);
};

