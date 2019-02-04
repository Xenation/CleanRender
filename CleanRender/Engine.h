#pragma once
class Window;
class Pipeline;
class EntityManager;
class Entity;
class Engine {
public:
	static Window* window;
	static Pipeline* pipeline;
	static EntityManager* entityManager;

	static void initialize();
	static void loop();
	static void destroy();
private:
	Engine();
	Engine(const Engine&);
	~Engine();

	static Entity* testEntity;
	static Entity* testCamera;
};

