#include "Renderer.h"

#include "Engine.h"
#include "Pipeline.h"
#include "ShaderProgram.h"



Renderer::Renderer(Entity* entity) : Component(entity), shaderProgram(ShaderProgram::errorShader) {
	id = Engine::pipeline->registerRenderer(this);
}

Renderer::~Renderer() {
	Engine::pipeline->unregisterRenderer(id);
}

void Renderer::setShaderProgram(ShaderProgram* program) {
	if (shaderProgram != nullptr) {
		shaderProgram->renderers.remove(spID);
	}
	if (program == nullptr) {
		shaderProgram = ShaderProgram::errorShader;
	} else {
		shaderProgram = program;
		spID = shaderProgram->renderers.add(this);
	}
}
