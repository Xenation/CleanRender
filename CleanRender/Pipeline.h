#pragma once
class Mesh;
class Pipeline {
public:
	Mesh* testMesh;
	Pipeline();
	~Pipeline();
	void render();
	void resizeFrameBuffer(int width, int height);
private:
	Pipeline(const Pipeline&) {};
};

