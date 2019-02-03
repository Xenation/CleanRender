#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"
#include "EntityManager.h"
#include "Entity.h"
#include "MeshRenderer.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;
EntityManager* Engine::entityManager = nullptr;
Entity* Engine::testEntity = nullptr;


void Engine::initialize() {
	window = new Window();
	pipeline = new Pipeline();
	entityManager = new EntityManager();
	testEntity = new Entity();
	MeshRenderer* mr = testEntity->addComponent<MeshRenderer>();
	mr->setMesh(pipeline->testMesh);
	mr->setShaderProgram(pipeline->testShader);
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
	delete testEntity;
}
