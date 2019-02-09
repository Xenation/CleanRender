#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"
#include "Input.h"
#include "Time.h"
#include "EntityManager.h"
#include "Scene.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;
EntityManager* Engine::entityManager = nullptr;
Scene* Engine::scene = nullptr;


void Engine::initialize() {
	window = new Window();
	pipeline = new Pipeline(window->getWidth(), window->getHeight());
	entityManager = new EntityManager();
}

void Engine::loop() {
	while (!window->shouldClose()) {
		Time::ComputeFrameTimes();
		// INPUT
		Input::PollEvents();
		// UPDATE
		entityManager->updateEntities();
		scene->update();
		// RENDER
		pipeline->render();
		// DISPLAY
		window->display();
	}
}

void Engine::destroy() {
	delete scene;
	delete entityManager;
	delete pipeline;
	delete window;
}
