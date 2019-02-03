#pragma once
#include "Component.h"
#include "Math.h"
struct PositionComponent : public Component<PositionComponent> {
	Vec3f position;
};

