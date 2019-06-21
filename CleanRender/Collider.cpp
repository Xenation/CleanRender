#include "Collider.h"

#include <bullet/btBulletCollisionCommon.h>



Collider* Collider::empty = new Collider(new btEmptyShape());


Collider::Collider() {}

Collider::Collider(btCollisionShape* shape) : shape(shape) {}

Collider::~Collider() {}
