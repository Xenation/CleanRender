#pragma once
class Window;
class Pipeline;
class EntityManager;
class Scene;
class Gui;
class Engine {
public:
	static Window* window;
	static Pipeline* pipeline;
	static EntityManager* entityManager;
	static Gui* gui;
	static Scene* scene;

	static void initialize(Pipeline* pipeline = nullptr, Gui* gui = nullptr);
	static void loop();
	static void destroy();
private:
	Engine();
	Engine(const Engine&);
	~Engine();
};

