#pragma once
#include "XMath.h"
#include "GLUtils.h"
#include "HollowSet.h"

class ShaderProgram;
class UniformBuffer;
class UniformLayout;
class Renderer;
class RenderPass;

class Material {
public:
	static Material* errorMaterial;

	ShaderProgram*const shaderProgram;
	HollowSet<Renderer*> renderers;

	Material(ShaderProgram* shaderProgram);
	~Material();

	void setField(unsigned int index, bool value);
	void setField(unsigned int index, int value);
	void setField(unsigned int index, unsigned int value);
	void setField(unsigned int index, float value);
	void setField(unsigned int index, double value);
	void setField(unsigned int index, Vec2f value);
	void setField(unsigned int index, Vec3f value);
	void setField(unsigned int index, Vec4f value);
	void setField(unsigned int index, Color value);
	void setField(unsigned int index, Matrix4x4f value);
	void updateFields();
	void use();

private:
	UniformBuffer* uniformBuffer;
	UniformLayout* uniformLayout;
	unsigned int idInProgram = 0;
	bool fieldsExpired = true;
};

