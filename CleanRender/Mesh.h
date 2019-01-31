#pragma once
#include "gl3w.h"
class Mesh {
public:
	Mesh(int vCount, int iCount);
	~Mesh();
	void setAttributesDefinition(int count, int* sizes);
	void setAttribute(int index, float* values);
	void setIndices(int* indices);
	void uploadToGL();
	void deleteFromGL();
	void render();
private:
	GLuint vao = 0;
	GLuint vboVertices = 0;
	GLuint vboIndices = 0;
	int attributeCount;
	int* attributeSizes;
	int* attributeOffsets;
	int vertexFloatSize;
	int vertexCount;
	float* vertices;
	int indexCount;
	int* indices;
};

