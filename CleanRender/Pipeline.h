#pragma once
#include <unordered_map>
class Mesh;
class ShaderProgram;
class Renderer;
class Pipeline {
public:
	Mesh* testMesh;
	Mesh* testMesh2;
	ShaderProgram* testShader;

	Pipeline();
	~Pipeline();

	void render();
	void resizeFrameBuffer(int width, int height);
	unsigned int registerRenderer(Renderer* renderer);
	void unregisterRenderer(unsigned int id);

private:
	std::unordered_map<unsigned int, Renderer*> renderers;
	unsigned int currentId = 0;

	Pipeline(const Pipeline&) = delete;
};

