#pragma once
#include "HollowSet.h"
class Mesh;
class ShaderProgram;
class Renderer;
class Camera;
class Pipeline {
public:
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

private:
	HollowSet<Renderer*> renderers;
	HollowSet<Camera*> cameras;
	float aspectRatio = 1.0f;

	void render(Camera* camera);
};

