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
	if (program == nullptr) {
		shaderProgram = ShaderProgram::errorShader;
	} else {
		shaderProgram = program;
	}
}
