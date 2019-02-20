#include "TestScene.h"

#include <Material.h>
#include "Mesh.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include "NoclipController.h"
#include "Time.h"



TestScene::TestScene() {}

TestScene::~TestScene() {}

void TestScene::load() {
	Scene::load();
	camera = new Entity("Camera");
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
	testMaterial = new Material(testShader);

	testCube = new Entity("TestCube");
	testCube->transform->setPosition(Vec3f(5, 0, 0));
	MeshRenderer* cubeRend = testCube->addComponent<MeshRenderer>();
	cubeRend->setMaterial(testMaterial);
	cubeRend->setMesh(cubeMesh);

	Entity* testCubeChild = new Entity("TestCubeChild");
	testCubeChild->setParent(testCube);
	testCubeChild->transform->setPosition(Vec3f(2, 0, 0));
	MeshRenderer* cubeRend2 = testCubeChild->addComponent<MeshRenderer>();
	cubeRend2->setMaterial(testMaterial);
	cubeRend2->setMesh(cubeMesh);

	noTransfParent = new Entity("NoTransfParent", false);
	transfChild = new Entity("TransfChild");
	transfChild->setParent(noTransfParent);
	transfChild->transform->setPosition(Vec3f(-5, 2, 0));
	MeshRenderer* transfRend = transfChild->addComponent<MeshRenderer>();
	transfRend->setMaterial(testMaterial);
	transfRend->setMesh(cubeMesh);

	Entity* subNoTransfChild = new Entity("SubNoTransfChild", false);
	subNoTransfChild->setParent(transfChild);

	Entity* subTransfChild = new Entity("SubTransfChild");
	subTransfChild->setParent(subNoTransfChild);
	subTransfChild->transform->setPosition(Vec3f(0, 2, 0));
	MeshRenderer* subTransfRend = subTransfChild->addComponent<MeshRenderer>();
	subTransfRend->setMaterial(testMaterial);
	subTransfRend->setMesh(cubeMesh);
}

void TestScene::update() {
	testCube->transform->rotate(Quaternion::euler(Vec3f(0, 5 * Time::deltaTime, 0)));
	transfChild->transform->rotate(Quaternion::euler(Vec3f(2.5f * Time::deltaTime, 0, 0)));
}

void TestScene::destroy() {
	Scene::destroy();
	delete camera;
	delete noTransfParent;
	delete testCube;
	delete cubeMesh;
	delete testShader;
}
