#pragma once
class Mesh;
class ShaderProgram;
class Pipeline {
public:
	Mesh* testMesh;
	Mesh* testMesh2;
	Pipeline();
	~Pipeline();
	void render();
	void resizeFrameBuffer(int width, int height);
private:
	ShaderProgram* testShader;

	Pipeline(const Pipeline&) {};
};

