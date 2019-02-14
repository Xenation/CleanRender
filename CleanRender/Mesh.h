#pragma once
#include "gl3w.h"
class Mesh {
public:
	Mesh(int vCount, int iCount);
	~Mesh();

	void setAttributesDefinition(int count, int* sizes);
	void setAttributesDefinition(int count, int* sizes, GLenum* types);
	void setAttribute(int index, signed char* values);
	void setAttribute(int index, char* values);
	void setAttribute(int index, short* values);
	void setAttribute(int index, unsigned short* values);
	void setAttribute(int index, int* values);
	void setAttribute(int index, unsigned int* values);
	void setAttribute(int index, float* values);
	void setAttribute(int index, double* values);
	void setAttributeData(int index, char* bytes);
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
	GLenum* attributeTypes = nullptr;
	unsigned int* attributeByteOffsets = nullptr;
	int vertexByteSize;
	int vertexCount;
	void* vertices = nullptr;
	int indexCount;
	unsigned int* indices = nullptr;
};

