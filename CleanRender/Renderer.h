#pragma once
#include "Component.h"
class ShaderProgram;
class Renderer : public Component {
public:
	Renderer(Entity* entity);
	~Renderer();

	virtual void render() = 0;

	void setShaderProgram(ShaderProgram* program) { shaderProgram = program; }
	ShaderProgram* getShaderProgram() { return shaderProgram; }

protected:
	ShaderProgram* shaderProgram = nullptr;

private:
	unsigned int id;
};

