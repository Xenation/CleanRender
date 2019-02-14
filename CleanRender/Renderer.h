#pragma once
#include "Component.h"
class ShaderProgram;
class Renderer : public Component {
public:
	bool isTransparent = false;

	Renderer(Entity* entity);
	~Renderer();

	virtual void render() = 0;

	void setShaderProgram(ShaderProgram* program);
	ShaderProgram* getShaderProgram() { return shaderProgram; }

protected:
	ShaderProgram* shaderProgram = nullptr;

private:
	unsigned int id;
	unsigned int spID;
};

