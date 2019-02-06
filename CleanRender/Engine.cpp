#include "Engine.h"

#include "Window.h"
#include "Pipeline.h"
#include "Input.h"
#include "EntityManager.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Camera.h" // TODO remove when not testing
#include "Transform.h"



Engine::Engine() {}
Engine::Engine(const Engine&) {}
Engine::~Engine() {}


Window* Engine::window = nullptr;
Pipeline* Engine::pipeline = nullptr;
EntityManager* Engine::entityManager = nullptr;
Entity* Engine::testEntity = nullptr;
Entity* Engine::testCamera = nullptr;


void Engine::initialize() {
	window = new Window();
	pipeline = new Pipeline(window->getWidth(), window->getHeight());
	entityManager = new EntityManager();
	testEntity = new Entity();
	MeshRenderer* mr = testEntity->addComponent<MeshRenderer>();
	mr->setMesh(pipeline->testMesh);
	mr->setShaderProgram(pipeline->testShader);
	testCamera = new Entity();
	testCamera->transform->setPosition({0, 0, -3});
	testCamera->addComponent<Camera>();
}

void Engine::loop() {
	while (!window->shouldClose()) {
		// INPUT
		Input::PollEvents();
		// UPDATE
		entityManager->updateEntities();
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
