#pragma once
#include <gl3w.h>
#include "GLUtils.h"
#include "XMath.h"

class UniformBuffer;

enum class UniformLayoutType {
	STD140,
	SHARED, // TODO Not yet supported
	PACKED, // TODO Not yet supported
};

struct UniformLayout {
	GLuint binding = 0;
	UniformLayoutType type = UniformLayoutType::STD140;
	GLSLType* members = nullptr;
	unsigned int* membersOffsets = nullptr;
	unsigned int memberCount = 0;
	unsigned int size = 0;
	unsigned int offset = 0;
	unsigned char* buffer = nullptr;

public:
	UniformLayout(unsigned int binding, unsigned int memberCount, GLSLType* members);
	UniformLayout(unsigned int binding, UniformLayoutType type, unsigned int memberCount, GLSLType* members);
	~UniformLayout();

	void setMember(unsigned int index, bool value);
	void setMember(unsigned int index, int value);
	void setMember(unsigned int index, unsigned int value);
	void setMember(unsigned int index, float value);
	void setMember(unsigned int index, double value);
	void setMember(unsigned int index, Vec2f value);
	void setMember(unsigned int index, Vec3f value);
	void setMember(unsigned int index, Vec4f value);
	void setMember(unsigned int index, Matrix4x4f value);
	void setMember(unsigned int index, unsigned char* bytes, unsigned int byteSize);
	void computeLayoutOffsets();
};

class UniformBuffer {
public:
	UniformBuffer();
	~UniformBuffer();

	void setLayouts(unsigned int layoutCount, UniformLayout* layouts);
	UniformLayout& getLayout(unsigned int index);
	void updateLayout(unsigned int index);
	void bindLayout(unsigned int index);
	void uploadToGL();
	void deleteFromGL();

private:
	GLuint ubo = 0;
	UniformLayout* layouts = nullptr;
	unsigned int layoutCount = 0;
	unsigned char* buffer = nullptr;
	unsigned int bufferSize = 0;
	bool loadedToGL = false;
};

