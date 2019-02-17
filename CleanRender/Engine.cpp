#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"
#include "Input.h"
#include "Time.h"
#include "EntityManager.h"
#include "Gui.h"
#include "Scene.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;
EntityManager* Engine::entityManager = nullptr;
Gui* Engine::gui = nullptr;
Scene* Engine::scene = nullptr;


void Engine::initialize(Pipeline* pipelinePt, Gui* guiPt) {
	window = new Window();
	entityManager = new EntityManager();
	if (pipelinePt == nullptr) {
		pipeline = new Pipeline(window->getWidth(), window->getHeight());
	} else {
		pipeline = pipelinePt;
	}
	if (guiPt == nullptr) {
		gui = new Gui(window);
	} else {
		gui = guiPt;
	}
}

void Engine::loop() {
	while (!window->shouldClose()) {
		Time::ComputeFrameTimes();
		// INPUT
		Input::PollEvents();
		// UPDATE
		entityManager->updateEntities();
		if (scene != nullptr) {
			scene->update();
		}
		gui->update();
		// RENDER
		pipeline->render();
		gui->render();
		// DISPLAY
		window->display();
	}
}

void Engine::destroy() {
	delete scene;
	delete entityManager;
	delete gui;
	delete pipeline;
	delete window;
}
