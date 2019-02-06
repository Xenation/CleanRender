#pragma once
#include "Scene.h"
class Mesh;
class Entity;
class ShaderProgram;
class TestScene : public Scene {
public:
	TestScene();
	~TestScene();

	virtual void load() override;
	virtual void destroy() override;
private:
	Entity* camera;
	Entity* testCube;
	Mesh* cubeMesh;
	ShaderProgram* testShader;
};

