#pragma once
#include <string>
#include "HollowSet.h"
#include "SimpleList.h"

class Mesh;
class Renderer;
class Camera;
class UniformBuffer;
class RenderPass;
class FrameBuffer;

class Pipeline {
public:
	SimpleList<RenderPass*> renderPasses;

	Pipeline(int width, int height);
	Pipeline(const Pipeline&) = delete;
	~Pipeline();

	void render();
	void resizeFrameBuffer(int width, int height);
	inline float getAspectRatio() { return aspectRatio; }

	unsigned int registerRenderer(Renderer* renderer);
	void unregisterRenderer(unsigned int id);
	unsigned int registerCamera(Camera* camera);
	void unregisterCamera(unsigned int id);

	RenderPass* getRenderPass(std::string name);

private:
	HollowSet<Renderer*> renderers;
	HollowSet<Camera*> cameras;
	UniformBuffer* globalUniformBuffer;
	FrameBuffer* renderBuffer;
	float aspectRatio = 1.0f;

	Mesh* fullscreenQuad;

	void render(Camera* camera);
};

