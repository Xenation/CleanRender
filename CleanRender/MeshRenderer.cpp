#include "MeshRenderer.h"

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"



MeshRenderer::MeshRenderer(Entity* entity) : Renderer(entity) {
	
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::render() {
	if (shaderProgram == nullptr || mesh == nullptr) return;

	shaderProgram->use();
	shaderProgram->loadModelMatrix(entity->transform->getLocalToWorldMatrix());

	mesh->render();

	shaderProgram->unuse();
}
