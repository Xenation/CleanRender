#pragma once
class Entity;
class BaseSystem;
class SystemManager {
public:
	static void registerSystem(BaseSystem* system);
	static void linkEntity(Entity* entity);
	static void unlinkEntity(Entity* entity);
private:
	static BaseSystem** systems;
	static unsigned int systemCount;
	static unsigned int systemCapacity;

	static void initializeSystemsArray();
	static void growSystemsArray();

	SystemManager() = delete;
	SystemManager(const SystemManager&) = delete;
	~SystemManager() = delete;
};

