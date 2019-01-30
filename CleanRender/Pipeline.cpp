#include "Pipeline.h"

#include "gl3w.h"



Pipeline::Pipeline() {
	glClearColor(1, 0, 0, 1);
}

Pipeline::~Pipeline() {}


void Pipeline::render() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Pipeline::resizeFrameBuffer(int width, int height) {
	glViewport(0, 0, width, height);
}
