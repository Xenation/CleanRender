#include "MeshRenderer.h"

#include "Mesh.h"
#include "Entity.h"
#include "Transform.h"
#include "Material.h"



MeshRenderer::MeshRenderer(Entity* entity) : Renderer(entity) {
	
}

MeshRenderer::~MeshRenderer() {

}

void MeshRenderer::render() {
	if (mesh == nullptr) return;

	material->setField(0, entity->transform->getLocalToWorldMatrix());
	material->updateFields();

	mesh->render();
}
