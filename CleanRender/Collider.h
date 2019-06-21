#pragma once

class btCollisionShape;

class Collider {
public:
	static Collider* empty;

	Collider();
	Collider(btCollisionShape* shape);
	~Collider();

	inline btCollisionShape* getBulletShape() { return shape; }

protected:
	btCollisionShape* shape = nullptr;
};

