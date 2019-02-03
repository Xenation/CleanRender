#include "SystemManager.h"

#include "Debug.h"
#include "BaseSystem.h"
#define SYSTEM_ARRAY_START_SIZE 16
#define SYSTEM_ARRAY_INCREASE 16



BaseSystem** SystemManager::systems = nullptr;
unsigned int SystemManager::systemCount = 0;
unsigned int SystemManager::systemCapacity = 0;

void SystemManager::registerSystem(BaseSystem* system) {
	if (systems == nullptr) {
		initializeSystemsArray();
	}
	if (systemCount == systemCapacity) {
		growSystemsArray();
		if (systemCount = systemCapacity) { // Grow failled
			return;
		}
	}
	system->id = systemCount;
	systems[system->id] = system;
	systemCount++;
}

//void SystemManager::unregisterSystem(BaseSystem* system) {
//	int i = 0;
//	for (i = system->id + 1; i < systemCount; i++) {
//		systems[i]->id--;
//		systems[i - 1] = systems[i];
//	}
//	systems[i - 1] = nullptr;
//	systems--;
//}

void SystemManager::linkEntity(Entity* entity) {

}

void SystemManager::unlinkEntity(Entity* entity) {

}

void SystemManager::initializeSystemsArray() {
	systemCapacity = SYSTEM_ARRAY_START_SIZE;
	systems = new BaseSystem*[systemCapacity];
}

void SystemManager::growSystemsArray() {
	systemCapacity += SYSTEM_ARRAY_INCREASE;
	BaseSystem** nSystems = new BaseSystem*[systemCapacity];
	if (nSystems == nullptr) {
		Debug::logError("EntityManager", "Could not allocate memory required for additionnal systems");
		return;
	}
	for (int i = 0; i < systemCount; i++) {
		nSystems[i] = systems[i];
	}
	delete[] systems;
	systems = nSystems;
}
