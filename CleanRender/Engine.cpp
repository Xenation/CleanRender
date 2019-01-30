#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;


void Engine::initialize() {
	window = new Window();
	pipeline = new Pipeline();
}

void Engine::loop() {
	while (!window->shouldClose()) {
		// INPUT
		// UPDATE
		// RENDER
		pipeline->render();
		// DISPLAY
		window->display();
	}
}

void Engine::destroy() {
	delete pipeline;
	delete window;
}
