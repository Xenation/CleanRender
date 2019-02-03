#include "Renderer.h"

#include "Engine.h"
#include "Pipeline.h"



Renderer::Renderer(Entity* entity) : Component(entity) {
	id = Engine::pipeline->registerRenderer(this);
}

Renderer::~Renderer() {
	Engine::pipeline->unregisterRenderer(id);
}
