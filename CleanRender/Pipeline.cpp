#include "Pipeline.h"

#include "gl3w.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"

#define RENDERERS_START_SIZE 32
#define RENDERERS_INCREASE 32
#define CAMERAS_START_SIZE 1
#define CAMERAS_INCREASE 1



Pipeline::Pipeline(int width, int height) 
	: renderers(HollowSet<Renderer*>(RENDERERS_START_SIZE, RENDERERS_INCREASE)), cameras(HollowSet<Camera*>(CAMERAS_START_SIZE, CAMERAS_INCREASE)){
	ShaderProgram::initializeAll();
	resizeFrameBuffer(width, height);
	glClearColor(0.5f, 0, 0, 1);
	glEnable(GL_CULL_FACE);
}

Pipeline::~Pipeline() {
	
}


void Pipeline::render() {
	unsigned int rendered = 0;
	for (unsigned int i = 0; i < cameras.capacity && rendered < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		render(cameras[i]);
		rendered++;
	}
}

void Pipeline::render(Camera* camera) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unsigned int rendered = 0;
	for (unsigned int i = 0; i < renderers.capacity && rendered < renderers.count; i++) {
		if (renderers[i] == nullptr) continue;
		renderers[i]->getShaderProgram()->use();
		renderers[i]->getShaderProgram()->loadProjectionMatrix(camera->getProjectionMatrix());
		renderers[i]->getShaderProgram()->loadViewMatrix(camera->getViewMatrix());
		renderers[i]->render();
		rendered++;
	}
}

void Pipeline::resizeFrameBuffer(int width, int height) {
	glViewport(0, 0, width, height);
	aspectRatio = ((float) width) / ((float) height);
	unsigned int adjusted = 0;
	for (unsigned int i = 0; i < cameras.capacity && adjusted < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		cameras[i]->updateProjectionMatrix();
		adjusted++;
	}
}

unsigned int Pipeline::registerRenderer(Renderer* renderer) {
	return renderers.add(renderer);
}

void Pipeline::unregisterRenderer(unsigned int id) {
	renderers.remove(id);
}

unsigned int Pipeline::registerCamera(Camera* camera) {
	return cameras.add(camera);
}

void Pipeline::unregisterCamera(unsigned int id) {
	cameras.remove(id);
}
