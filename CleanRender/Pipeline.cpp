#include "Pipeline.h"

#include <locale>
#include <gl3w.h>
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Camera.h"
#include "Time.h"
#include "UniformBuffer.h"
#include "Material.h"
#include "RenderPass.h"

#define RENDERERS_START_SIZE 32
#define RENDERERS_INCREASE 32
#define CAMERAS_START_SIZE 1
#define CAMERAS_INCREASE 1
#define RENDERPASSES_START_SIZE 4
#define RENDERPASSES_INCREASE 1



Pipeline::Pipeline(int width, int height) 
	: renderers(RENDERERS_START_SIZE, RENDERERS_INCREASE), cameras(CAMERAS_START_SIZE, CAMERAS_INCREASE), renderPasses(RENDERPASSES_START_SIZE, RENDERERS_INCREASE) {
	renderPasses.add(new RenderPassOpaque("opaque"));
	renderPasses.add(new RenderPassTransparent("transparent"));
	ShaderProgram::defaultPipeline = this;
	ShaderProgram::initializeAll();
	resizeFrameBuffer(width, height);
	globalUniformBuffer = new UniformBuffer();
	globalUniformBuffer->setLayouts(2, new UniformLayout[2]{UniformLayout(1, 2, new GLSLType[2]{GLSL_MAT4, GLSL_MAT4}), UniformLayout(2, 1, new GLSLType[1]{GLSL_FLOAT})});
	globalUniformBuffer->uploadToGL();
	glClearColor(0.5f, 0, 0, 1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

Pipeline::~Pipeline() {
	delete globalUniformBuffer;
}


void Pipeline::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	unsigned int rendered = 0;
	for (unsigned int i = 0; i < cameras.capacity && rendered < cameras.count; i++) {
		if (cameras[i] == nullptr) continue;
		render(cameras[i]);
		rendered++;
	}
}

void Pipeline::render(Camera* camera) {
	// Globals Update
	globalUniformBuffer->getLayout(0).setMember(0, camera->getProjectionMatrix());
	globalUniformBuffer->getLayout(0).setMember(1, camera->getViewMatrix());
	globalUniformBuffer->updateLayout(0);
	globalUniformBuffer->getLayout(1).setMember(0, Time::time);
	globalUniformBuffer->updateLayout(1);

	for (unsigned int passIndex = 0; passIndex < renderPasses.count; passIndex++) {
		RenderPass* renderPass = renderPasses[passIndex];
		renderPass->preparePass();
		unsigned int shadersRendered = 0;
		for (unsigned int shaderIndex = 0; shaderIndex < renderPass->programs.capacity && shadersRendered < renderPass->programs.count; shaderIndex++) {
			ShaderProgram* shaderProgram = renderPass->programs[shaderIndex];
			if (shaderProgram == nullptr) continue;
			shaderProgram->use();
			unsigned int materialsRendered = 0;
			for (unsigned int materialIndex = 0; materialIndex < shaderProgram->materials.capacity && materialsRendered < shaderProgram->materials.count; materialIndex++) {
				Material* material = shaderProgram->materials[materialIndex];
				if (material == nullptr) continue;
				material->use();
				unsigned int renderersRendered = 0;
				for (unsigned int rendererIndex = 0; rendererIndex < material->renderers.capacity && rendererIndex < material->renderers.count; rendererIndex++) {
					Renderer* renderer = material->renderers[rendererIndex];
					if (renderer == nullptr) continue;
					renderer->render();
				}
			}
		}
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

RenderPass* Pipeline::getRenderPass(std::string name) {
	for (int i = 0; i < renderPasses.count; i++) {
		if (renderPasses[i]->name == name) {
			return renderPasses[i];
		}
	}
	return nullptr;
}
