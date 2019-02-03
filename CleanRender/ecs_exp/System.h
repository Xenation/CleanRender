#pragma once
#include "BaseSystem.h"
#include "SystemManager.h"
template<typename Derived>
class System : public BaseSystem {
public:
	static void* initialize() {
		SystemManager::registerSystem(this);
	}

	System() {
		si;
	}
private:
	static void* si; // static initializer
};

template<typename Derived>
void* System<Derived>::si = System<Derived>::initialize();

