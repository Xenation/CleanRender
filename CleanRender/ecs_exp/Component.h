#pragma once
#include "BaseComponent.h"

template<typename Derived>
struct Component : public BaseComponent {
public:
	static unsigned int typeId;

	Component() {
		typeId;
	}
protected:
	static unsigned int getTypeId() {
		unsigned int tid = BaseComponent::typesCount++;
		BaseComponent::typesBitFieldSize = (BaseComponent::typesCount / 8) + (BaseComponent::typesCount % 8 > 0);
		return tid;
	}
};

template<typename Derived>
unsigned int Component<Derived>::typeId = Component<Derived>::getTypeId();

