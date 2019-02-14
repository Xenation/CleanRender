#include "Pipeline.h"

#include "gl3w.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"
#include "Time.h"

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
	glEnable(GL_DEPTH_TEST);
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
	// Opaque Pass
	glDisable(GL_BLEND);
	for (unsigned int shaderIndex = 0; shaderIndex < ShaderProgram::shaderCount; shaderIndex++) {
		ShaderProgram* shaderProgram = ShaderProgram::shaders[shaderIndex];
		shaderProgram->use();
		shaderProgram->loadProjectionMatrix(camera->getProjectionMatrix());
		shaderProgram->loadViewMatrix(camera->getViewMatrix());
		shaderProgram->loadTime(Time::time);
		unsigned int rendered = 0;
		for (unsigned int rendererIndex = 0; rendererIndex < shaderProgram->renderers.capacity && rendered < shaderProgram->renderers.count; rendererIndex++) {
			if (shaderProgram->renderers[rendererIndex] == nullptr || shaderProgram->renderers[rendererIndex]->isTransparent) continue;
			shaderProgram->renderers[rendererIndex]->render();
			rendered++;
		}
		shaderProgram->unuse();
	}

	// Tranparent Pass
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (unsigned int shaderIndex = 0; shaderIndex < ShaderProgram::shaderCount; shaderIndex++) {
		ShaderProgram* shaderProgram = ShaderProgram::shaders[shaderIndex];
		shaderProgram->use();
		shaderProgram->loadProjectionMatrix(camera->getProjectionMatrix());
		shaderProgram->loadViewMatrix(camera->getViewMatrix());
		shaderProgram->loadTime(Time::time);
		unsigned int rendered = 0;
		for (unsigned int rendererIndex = 0; rendererIndex < shaderProgram->renderers.capacity && rendered < shaderProgram->renderers.count; rendererIndex++) {
			if (shaderProgram->renderers[rendererIndex] == nullptr || !shaderProgram->renderers[rendererIndex]->isTransparent) continue;
			shaderProgram->renderers[rendererIndex]->render();
			rendered++;
		}
		shaderProgram->unuse();
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
