#pragma once
class Window;
class Pipeline;
class Engine {
public:
	static Window* window;
	static Pipeline* pipeline;

	static void initialize();
	static void loop();
	static void destroy();
private:
	Engine();
	Engine(const Engine&);
	~Engine();
};

