#include "TestScene.h"

#include "Mesh.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "NoclipController.h"



TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::load() {
	Scene::load();
	camera = new Entity();
	camera->addComponent<Camera>();
	camera->addComponent<NoclipController>();
	camera->transform->setPosition({0, 0, -5});

	cubeMesh = new Mesh(8, 36);
	cubeMesh->setAttributesDefinition(1, new int[1]{3});
	cubeMesh->setAttribute(0, new float[24]{
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
	});
	cubeMesh->setIndices(new unsigned int[36]{
		0, 2, 1,
		0, 3, 2,
		4, 3, 0,
		4, 7, 3,
		5, 0, 1,
		5, 4, 0,
		6, 1, 2,
		6, 5, 1,
		7, 2, 3,
		7, 6, 2,
		5, 7, 4,
		5, 6, 7
	});
	cubeMesh->uploadToGL();

	testShader = ShaderProgram::find("test");
	if (testShader != nullptr) {
		testShader->load();
	}

	testCube = new Entity();
	MeshRenderer* cubeRend = testCube->addComponent<MeshRenderer>();
	cubeRend->setShaderProgram(testShader);
	cubeRend->setMesh(cubeMesh);
}

void TestScene::destroy() {
	Scene::destroy();
	delete camera;
	delete testCube;
	delete cubeMesh;
	delete testShader;
}
