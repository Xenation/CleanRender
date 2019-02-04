#pragma once
#include "HollowSet.h"
class Mesh;
class ShaderProgram;
class Renderer;
class Camera;
class Pipeline {
public:
	Mesh* testMesh;
	Mesh* testMesh2;
	ShaderProgram* testShader;

	Pipeline();
	Pipeline(const Pipeline&) = delete;
	~Pipeline();

	void render();
	void resizeFrameBuffer(int width, int height);
	unsigned int registerRenderer(Renderer* renderer);
	void unregisterRenderer(unsigned int id);
	unsigned int registerCamera(Camera* camera);
	void unregisterCamera(unsigned int id);

private:
	HollowSet<Renderer*> renderers;
	HollowSet<Camera*> cameras;

	void render(Camera* camera);
};

