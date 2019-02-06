#include "Mesh.h"



Mesh::Mesh(int vCount, int iCount) {
	vertexCount = vCount;
	indexCount = iCount;
	indices = new unsigned int[indexCount];
}

Mesh::~Mesh() {
	if (loadedToGL) {
		deleteFromGL();
	}
	if (attributeSizes != nullptr) {
		delete[] attributeSizes;
	}
	if (attributeFloatOffsets != nullptr) {
		delete[] attributeFloatOffsets;
	}
	if (vertices != nullptr) {
		delete[] vertices;
	}
	delete[] indices;
}

void Mesh::setAttributesDefinition(int count, int* sizes) {
	attributeCount = count;
	attributeSizes = sizes;
	attributeFloatOffsets = new int[count];
	vertexFloatSize = 0;
	for (int i = 0; i < attributeCount; i++) {
		attributeFloatOffsets[i] = vertexFloatSize;
		vertexFloatSize += attributeSizes[i];
	}
	vertices = new float[vertexCount * vertexFloatSize];
}

void Mesh::setAttribute(int index, float* values) {
	int attrFloatOffset = attributeFloatOffsets[index];
	int attrSize = attributeSizes[index];
	for (int vi = 0; vi < vertexCount; vi++) {
		for (int i = 0; i < attrSize; i++) {
			vertices[vi * vertexFloatSize + attrFloatOffset + i] = values[vi * attrSize + i];
		}
	}
}

void Mesh::setIndices(unsigned int* indices) {
	this->indices = indices;
}

void Mesh::uploadToGL() {
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	for (int i = 0; i < attributeCount; i++) {
		glEnableVertexAttribArray(i);
	}

	if (vboVertices == 0) {
		glGenBuffers(1, &vboVertices);
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	unsigned int vboVerticesSize = vertexCount * vertexFloatSize * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, vboVerticesSize, vertices, GL_STATIC_DRAW);
	for (int i = 0; i < attributeCount; i++) {
		glVertexAttribPointer(i, attributeSizes[i], GL_FLOAT, GL_FALSE, vertexFloatSize * sizeof(float), (void*) (attributeFloatOffsets[i] * sizeof(float)));
	}

	if (vboIndices == 0) {
		glGenBuffers(1, &vboIndices);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	unsigned int vboIndicesSize = indexCount * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndicesSize, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Not unbinding the element array since it is bound to the vao
	loadedToGL = true;
}

void Mesh::deleteFromGL() {
	if (vboIndices != 0) {
		glDeleteBuffers(1, &vboIndices);
		vboIndices = 0;
	}
	if (vboVertices != 0) {
		glDeleteBuffers(1, &vboVertices);
		vboVertices = 0;
	}
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
}

void Mesh::render() {
	if (!loadedToGL) return;

	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);
}
