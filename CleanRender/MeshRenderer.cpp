#include "MeshRenderer.h"

#include "ShaderProgram.h"
#include "Mesh.h"



MeshRenderer::MeshRenderer(Entity* entity) : Renderer(entity) {
	
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::render() {
	if (shaderProgram == nullptr || mesh == nullptr) return;

	shaderProgram->use();

	mesh->render();

	shaderProgram->unuse();
}
