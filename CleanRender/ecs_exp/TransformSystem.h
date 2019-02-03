#pragma once
#include "System.h"
class TransformSystem : System<TransformSystem> {
public:
	TransformSystem();
	~TransformSystem();

	virtual void update();
};

