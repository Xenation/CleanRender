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
	if (mesh == nullptr) return;

	shaderProgram->loadModelMatrix(entity->transform->getLocalToWorldMatrix());

	mesh->render();
}
