#include "Pipeline.h"

#include "gl3w.h"
#include "Mesh.h"
#include "ShaderProgram.h"



Pipeline::Pipeline() {
	ShaderProgram::initializeAll();
	glClearColor(1, 0, 0, 1);
	testMesh = new Mesh(3, 3);
	testMesh->setAttributesDefinition(1, new int[1] {3});
	testMesh->setAttribute(0, new float[9] {0, 0, 0, 1, 1, 0, 0, 1, 0});
	testMesh->setIndices(new int[3] {0, 1, 2});
	testMesh->uploadToGL();
}

Pipeline::~Pipeline() {
	delete testMesh;
}


void Pipeline::render() {
	testMesh->render();

	glClear(GL_COLOR_BUFFER_BIT);
}

void Pipeline::resizeFrameBuffer(int width, int height) {
	glViewport(0, 0, width, height);
}
