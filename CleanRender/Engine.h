#pragma once
class Window;
class Pipeline;
class EntityManager;
class Scene;
class Entity;
class Engine {
public:
	static Window* window;
	static Pipeline* pipeline;
	static EntityManager* entityManager;
	static Scene* scene;

	static void initialize();
	static void loop();
	static void destroy();
private:
	Engine();
	Engine(const Engine&);
	~Engine();
};

