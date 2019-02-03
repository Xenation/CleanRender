#include "Pipeline.h"

#include "gl3w.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Renderer.h"



Pipeline::Pipeline() {
	ShaderProgram::initializeAll();
	testShader = ShaderProgram::find("test");
	if (testShader != nullptr) {
		testShader->load();
	}
	glClearColor(0.5f, 0, 0, 1);
	testMesh = new Mesh(3, 3);
	testMesh->setAttributesDefinition(1, new int[1] {3});
	testMesh->setAttribute(0, new float[9] {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f});
	testMesh->setIndices(new unsigned int[3] {0, 1, 2});
	testMesh->uploadToGL();
	testMesh2 = new Mesh(4, 3);
	testMesh2->setAttributesDefinition(1, new int[1]{3});
	testMesh2->setAttribute(0, new float[12]{-1.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
	testMesh2->setIndices(new unsigned int[3]{3, 2, 0});
	testMesh2->uploadToGL();
	renderers = std::unordered_map<unsigned int, Renderer*>();
}

Pipeline::~Pipeline() {
	delete testMesh;
	delete testMesh2;
}


void Pipeline::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//testShader->use();

	//testMesh->render();
	//testMesh2->render();

	for (std::pair<unsigned int, Renderer*> p : renderers) {
		p.second->render();
	}

	//testShader->unuse();
}

void Pipeline::resizeFrameBuffer(int width, int height) {
	glViewport(0, 0, width, height);
}

unsigned int Pipeline::registerRenderer(Renderer* renderer) {
	int id = currentId++;
	renderers[id] = renderer;
	return id;
}

void Pipeline::unregisterRenderer(unsigned int id) {
	renderers.erase(id);
}
