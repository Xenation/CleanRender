#pragma once
#include "gl3w.h"
class Mesh {
public:
	Mesh(int vCount, int iCount);
	~Mesh();

	void setAttributesDefinition(int count, int* sizes);
	void setAttribute(int index, float* values);
	void setIndices(unsigned int* indices);
	void deleteLocal();
	void uploadToGL();
	void deleteFromGL();
	void render() const;
	inline bool isLoadedToGL() const { return loadedToGL; }
	inline bool isCachedInLocal() const { return cachedInLocal; }

private:
	GLuint vao = 0;
	GLuint vboVertices = 0;
	GLuint vboIndices = 0;
	bool loadedToGL = false;
	bool cachedInLocal = false;
	int attributeCount;
	int* attributeSizes = nullptr;
	int* attributeFloatOffsets = nullptr;
	int vertexFloatSize;
	int vertexCount;
	float* vertices = nullptr;
	int indexCount;
	unsigned int* indices = nullptr;
};

